#include "Render/RenderGraph/RenderGraphCompiler.h"
#include "Core/Algo/GraphSearch.h"
#include "Core/Algo/TopologicalSort.h"
#include "Render/RenderGraph.h"

RenderGraphCompiler::RenderGraphCompiler(RenderGraph &graph, const CompileContext &ctx)
    : graph(graph), ctx(ctx)
{
}

SPtr<RenderGraphExecutor> RenderGraphCompiler::Compile(RenderGraph &graph, const CompileContext &ctx)
{
    RenderGraphCompiler compiler(graph, ctx);

    auto resourceCache = RenderGraphResourceCache::Create();
    for (auto &[n, r] : ctx.externalResources)
    {
        resourceCache->RegisterExternalResource(n, r);
    }

    compiler.ResolveExecutionOrder();
    compiler.CompilePasses();
    if (compiler.InsertAutoPasses())
        compiler.ResolveExecutionOrder();
    compiler.ValidateGraph();
    compiler.AllocateResources(resourceCache.get());

    auto executor = RenderGraphExecutor::Create();
    executor->resourceCache = resourceCache;
    executor->executionList = std::move(compiler.executionList);

    compiler.RestoreCompilationChanges();

    return executor;
}

void RenderGraphCompiler::ResolveExecutionOrder()
{
    executionList.Clear();

    HashSet<int32> mandatoryPasses;
    for (auto &o : graph.outputs)
    {
        mandatoryPasses.Add(o.nodeID);
    }
    for (auto &[i, e] : graph.graph.GetEdges())
    {
        if (e.dst.fieldName.IsEmpty())
        {
            CT_CHECK(e.src.fieldName.IsEmpty());
            mandatoryPasses.Add(e.dst.nodeID);
            mandatoryPasses.Add(e.src.nodeID);
        }
    }

    HashSet<int32> participatingPasses;
    for (auto nodeID : mandatoryPasses)
    {
        Algo::DepthFirstSearch(
            graph.graph, nodeID, [&participatingPasses](int32 i) { participatingPasses.Add(i); }, false);
    }

    auto sortResult = Algo::TopologicalSort(graph.graph);
    for (int32 nodeID : sortResult)
    {
        if (participatingPasses.Contains(nodeID))
        {
            const auto &node = graph.graph.GetNode(nodeID);
            executionList.Add({
                .nodeID = nodeID,
                .name = node.name,
                .pass = node.pass,
                .reflection = node.pass->Reflect(ctx),
            });
        }
    }
}

void RenderGraphCompiler::CompilePasses()
{
    //TODO Prepare pass compile data

    for (auto &e : executionList)
    {
        e.pass->Compile(ctx);
    }
}

bool RenderGraphCompiler::InsertAutoPasses()
{
    bool changed = false;

    auto &innerGraph = graph.graph;
    for (int32 i = 0; i < executionList.Count(); ++i)
    {
        const auto &reflection = executionList[i].reflection;

        // Iterate output
        for (int32 f = 0; f < reflection.GetFieldCount(); ++f)
        {
            auto &srcField = reflection.GetField(f);
            if (!srcField.IsOutput())
                continue;

            const String &srcPassName = executionList[i].name;
            String srcFieldName = srcPassName + CT_TEXT(".") + srcField.GetName();

            for (int32 e : innerGraph.GetForwardEdges(executionList[i].nodeID))
            {
                const auto &edge = innerGraph.GetEdge(e);

                if (srcField.GetName() == edge.src.fieldName)
                {
                    const String &dstPassName = innerGraph.GetNode(edge.dst.nodeID).name;

                    int32 idx = -1;
                    for (int32 ii = 0; ii < executionList.Count(); ++ii)
                    {
                        if (executionList[ii].nodeID == edge.dst.nodeID)
                        {
                            idx = ii;
                            break;
                        }
                    }

                    if (idx == -1)
                        continue;

                    const auto &dstField = reflection.GetField(edge.dst.fieldName);
                    CT_CHECK(srcField.IsValid());
                    CT_CHECK(dstField.IsValid());


                    // TODO Can auto resovle ...
                }
            }

            //TODO Add auto pass
        }
    }

    return changed;
}

bool RenderGraphCompiler::ValidateGraph()
{
    for (const auto &e : executionList)
    {
        for (int32 i = 0; i < e.reflection.GetFieldCount(); ++i)
        {
            auto &field = e.reflection.GetField(i);
            if (!field.IsInput())
                continue;
            if (field.IsOptional())
                continue;

            const String &name = field.GetName();
            bool found = false;
            for (int32 edgeID : graph.graph.GetBackwardEdges(e.nodeID))
            {
                if (graph.graph.GetEdge(edgeID).dst.fieldName == name)
                {
                    found = true;
                    break;
                }
            }

            String resName = e.name + CT_TEXT(".") + name;
            bool hasExternal = ctx.externalResources.Contains(resName);

            if (hasExternal && found)
            {
                CT_EXCEPTION(Render, "Input field can not have both bound external resource and incoming edge.");
                return false;
            }
            if (!hasExternal && !found)
            {
                CT_EXCEPTION(Render, "Input field resource is required.");
                return false;
            }
        }
    }

    if (graph.GetOutputCount() == 0)
    {
        CT_EXCEPTION(Render, "Graph must have at least one output");
        return false;
    }

    return true;
}

void RenderGraphCompiler::AllocateResources(RenderGraphResourceCache *resourceCache)
{
    // HashMap<RenderPass *, int32> passToIndex;
    // for (int32 i = 0; i < executionList.Count(); ++i)
    // {
    //     passToIndex.Put(executionList[i].pass.get(), i);
    // }

    for (int32 i = 0; i < executionList.Count(); ++i)
    {
        int32 nodeID = executionList[i].nodeID;
        const auto &node = graph.graph.GetNode(nodeID);
        auto &reflection = executionList[i].reflection;

        auto IsResourceUsed = [&, this](const auto &field) {
            if (field.IsOptional())
                return true;
            if (graph.GetOutputIndex({ nodeID, field.GetName() }) != -1)
                return true;
            for (int32 e : graph.graph.GetForwardEdges(nodeID))
            {
                const auto &edge = graph.graph.GetEdge(e);
                if (edge.src.fieldName == field.GetName())
                    return true;
            }
            return false;
        };

        // Register pass outputs
        for (int32 f = 0; f < reflection.GetFieldCount(); ++f)
        {
            auto field = reflection.GetField(f); //FIXME Use reference?
            String fullName = node.name + CT_TEXT(".") + field.GetName();
            if (!field.IsInput())
            {
                if (IsResourceUsed(field) == false)
                    continue;

                bool graphOut = graph.GetOutputIndex({ nodeID, field.GetName() }) != -1;
                if (graphOut && field.GetResourceBindFlags() != ResourceBind::None)
                    field.SetResourceBindFlags(field.GetResourceBindFlags() | ResourceBind::ShaderResource);

                resourceCache->RegisterField(fullName, field);
            }
        }

        // Go over pass inputs, add as aliases to the connected outputs.
        for (int32 e : graph.graph.GetBackwardEdges(nodeID))
        {
            const auto &edge = graph.graph.GetEdge(e);
            
            if (edge.dst.fieldName.IsEmpty())
            {
                CT_CHECK(edge.src.fieldName.IsEmpty());
                continue;
            }

            const auto &dstField = reflection.GetField(edge.dst.fieldName);
            CT_CHECK(dstField.IsValid() && dstField.IsInput());

            const auto &srcNode = graph.graph.GetNode(edge.src.nodeID);
            String srcFieldName = srcNode.name + CT_TEXT(".") + edge.src.fieldName;
            String dstFieldName = node.name + CT_TEXT(".") + edge.dst.fieldName;

            resourceCache->RegisterFieldAlias(srcFieldName, dstFieldName, dstField);
        }
    }

    resourceCache->AllocateResources(ctx.defaultResourceProps);
}

void RenderGraphCompiler::RestoreCompilationChanges()
{
    //TODO
}
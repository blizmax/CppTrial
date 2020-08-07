#include "Render/RenderGraph/RenderGraphCompiler.h"
#include "Core/Algo/GraphSearch.h"
#include "Core/Algo/TopologicalSort.h"
#include "Render/RenderGraph.h"
#include "Render/RenderGraph/RenderGraphExecutor.h"

RenderGraphCompiler::RenderGraphCompiler(RenderGraph &graph)
    : graph(graph)
{
}

SPtr<RenderGraphExecutor> RenderGraphCompiler::Compile(RenderGraph &graph, const RenderGraph::CompileContext &ctx)
{
    RenderGraphCompiler compiler(graph);

    auto resourceCache = RenderGraphResourceCache::Create();
    for (auto &[n, r] : ctx.externalResources)
    {
        resourceCache->RegisterExternalResource(n, r);
    }

    compiler.ResolveExecutionOrder(ctx);
    compiler.CompilePasses(ctx);
    if (compiler.InsertAutoPasses())
        compiler.ResolveExecutionOrder(ctx);

    //TODO

    return nullptr;
}

void RenderGraphCompiler::ResolveExecutionOrder(const CompileContext &ctx)
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

void RenderGraphCompiler::CompilePasses(const CompileContext &ctx)
{
    //TODO while(true) ...

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

void RenderGraphCompiler::ValidateGraph()
{
    //TODO
}
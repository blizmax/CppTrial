#include "Render/RenderGraph.h"
#include "Render/RenderGraph/RenderGraphCompiler.h"
#include "Render/RenderGraph/RenderGraphExecutor.h"
#include "Render/RenderPass.h"
#include "Render/Scene.h"

SPtr<RenderGraph> RenderGraph::Create()
{
    return Memory::MakeShared<RenderGraph>();
}

RenderGraph::RenderGraph()
{
}

RenderGraph::NodePort RenderGraph::ParseNodePort(const String &fullname) const
{
    NodePort port;

    int32 index = fullname.IndexOf(CT_TEXT("."));
    if (index != -1)
    {
        port.nodeID = GetPassIndex(fullname.Substring(0, index));
        port.fieldName = fullname.Substring(index + 1);
    }
    else
    {
        port.nodeID = GetPassIndex(fullname);
    }
    return port;
}

bool RenderGraph::CheckNodePortField(const NodePort &port, bool input) const
{
    if (port.nodeID == -1)
        return false;

    if (port.fieldName.IsEmpty())
        return true;

    // TODO
    return true;
}

void RenderGraph::SetScene(const SPtr<Scene> &inScene)
{
    if (scene == inScene)
        return;

    scene = inScene;
    for (auto &[i, n] : graph.GetNodes())
    {
        n.pass->SetScene(scene);
    }

    recompile = true;
}

int32 RenderGraph::GetPassIndex(const String &name) const
{
    auto ptr = passNameToIndex.TryGet(name);
    return ptr ? *ptr : -1;
}

SPtr<RenderPass> RenderGraph::GetPass(const String &name) const
{
    int32 passIndex = GetPassIndex(name);
    if (passIndex == -1)
    {
        CT_LOG(Warning, CT_TEXT("Get pass failed, pass named {0} doesn't exist."), name);
        return nullptr;
    }
    return graph.GetNode(passIndex).pass;
}

int32 RenderGraph::AddPass(const String &name, const SPtr<RenderPass> &pass)
{
    int32 passIndex = GetPassIndex(name);
    if (passIndex != -1)
    {
        CT_LOG(Warning, CT_TEXT("Add pass failed, pass named {0} already exists."), name);
        return -1;
    }

    passIndex = graph.AddNode({ name, pass });
    passNameToIndex.Put(name, passIndex);

    // TODO reset callback
    pass->SetName(name);
    pass->SetScene(scene);

    recompile = true;

    return passIndex;
}

bool RenderGraph::RemovePass(const String &name)
{
    int32 passIndex = GetPassIndex(name);

    if (passIndex == -1)
    {
        CT_LOG(Warning, CT_TEXT("Remove pass failed, pass named {0} doesn't exist."), name);
        return false;
    }

    graph.RemoveNode(passIndex);
    passNameToIndex.Remove(name);
    UnmarkOutputs(passIndex);

    recompile = true;

    return true;
}

int32 RenderGraph::GetEdgeIndex(const String &src, const String &dst) const
{
    auto srcPort = ParseNodePort(src);
    if (srcPort.nodeID == -1)
        return -1;
    auto dstPort = ParseNodePort(dst);
    if (dstPort.nodeID == -1)
        return -1;

    for (int32 edgeID : graph.GetForwardEdges(srcPort.nodeID))
    {
        auto &e = graph.GetEdge(edgeID);
        if (e.src == srcPort && e.dst == dstPort)
            return edgeID;
    }
    return -1;
}

int32 RenderGraph::AddEdge(const String &src, const String &dst)
{
    auto srcPort = ParseNodePort(src);
    if (srcPort.nodeID == -1)
        return -1;
    auto dstPort = ParseNodePort(dst);
    if (dstPort.nodeID == -1)
        return -1;

    // Avoid circular connection
    if (graph.ContainsEdge(dstPort.nodeID, srcPort.nodeID))
    {
        CT_LOG(Warning, CT_TEXT("RenderGraph::AddEdge() try to make a circular connection from {0} to {1}."), src, dst);
        return -1;
    }

    // If this is a data edge, make sure dst field is not already initialized
    if (!dstPort.fieldName.IsEmpty())
    {
        for (int32 e : graph.GetBackwardEdges(dstPort.nodeID))
        {
            const auto &edge = graph.GetEdge(e);
            if (edge.dst == dstPort)
            {
                CT_LOG(Error, CT_TEXT("RenderGraph::AddEdge() destination {0} is already initialized, remove that edge first."), dst);
                return -1;
            }
        }
    }

    //Check field in certain pass exists
    if (!CheckNodePortField(srcPort, false))
        return -1;
    if (!CheckNodePortField(dstPort, true))
        return -1;

    Edge edge = { srcPort, dstPort };
    int32 ret = graph.AddEdge(std::move(edge));

    recompile = true;
    return ret;
}

bool RenderGraph::RemoveEdge(const String &src, const String &dst)
{
    int32 edgeID = GetEdgeIndex(src, dst);
    if (edgeID == -1)
        return false;

    return RemoveEdge(edgeID);
}

bool RenderGraph::RemoveEdge(int32 edgeID)
{
    bool ret = graph.RemoveEdge(edgeID);
    if (ret)
        recompile = true;
    return ret;
}

void RenderGraph::SetInput(const String &name, const SPtr<Resource> &resource)
{
    auto port = ParseNodePort(name);
    if (port.nodeID == -1)
    {
        CT_LOG(Warning, CT_TEXT("RenderGraph::SetInput() can not find pass, name:{0}."), name);
        return;
    }
    if (!CheckNodePortField(port, true))
    {
        CT_LOG(Warning, CT_TEXT("RenderGraph::SetInput() field is invalid, name:{0}."), name);
        return;
    }

    if (resource)
    {
        externalResources.Put(name, resource);
    }
    else
    {
        if (externalResources.Contains(name))
        {
            CT_LOG(Warning, CT_TEXT("RenderGraph::SetInput() try to remove an external resource, name:{0}."), name);
            return;
        }
    }

    if (executor)
    {
        executor->SetInput(name, resource);
    }
}

int32 RenderGraph::GetOutputIndex(const NodePort &port) const
{
    if (port.nodeID == -1)
        return -1;

    for (int32 i = 0; i < outputs.Count(); ++i)
    {
        if (outputs[i] == port)
            return i;
    }
    return -1;
}

bool RenderGraph::IsOutput(const String &name) const
{
    auto port = ParseNodePort(name);
    return GetOutputIndex(port) != -1;
}

SPtr<Resource> RenderGraph::GetOutput(const String &name) const
{
    if (recompile)
    {
        CT_LOG(Error, CT_TEXT("RenderGraph::GetOutput() try fetch output before graph compiled."));
        return nullptr;
    }

    if (!IsOutput(name))
        return nullptr;

    CT_CHECK(executor);
    return executor->GetResource(name);
}

void RenderGraph::MarkOutput(const String &name)
{
    auto port = ParseNodePort(name);
    if (port.nodeID == -1)
    {
        CT_LOG(Warning, CT_TEXT("RenderGraph::MarkOutput() can not find pass, name:{0}."), name);
        return;
    }
    if (!CheckNodePortField(port, false))
    {
        CT_LOG(Warning, CT_TEXT("RenderGraph::MarkOutput() field is invalid, name:{0}."), name);
        return;
    }

    if (GetOutputIndex(port) == -1)
    {
        outputs.Add(port);
        recompile = true;
    }
}

void RenderGraph::UnmarkOutputs(int32 nodeID)
{
    int32 curIndex = 0;
    while (curIndex < outputs.Count())
    {
        for (int32 i = curIndex; i < outputs.Count(); ++i)
        {
            if (outputs[i].nodeID == nodeID)
            {
                outputs.RemoveAt(i);
                curIndex = i;
                break;
            }
        }
    }
}

void RenderGraph::UnmarkOutput(const String &name)
{
    auto port = ParseNodePort(name);
    int32 outputIndex = GetOutputIndex(port);
    if (outputIndex != -1)
    {
        outputs.RemoveAt(outputIndex);
        recompile = true;
    }
}

bool RenderGraph::Compile(RenderContext *ctx)
{
    if (!recompile)
        return true;

    recompile = false;

    executor = RenderGraphCompiler::Compile(
        *this,
        {
            .renderContext = ctx,
            .defaultResourceProps = defaultResourceProps,
            .externalResources = externalResources,
        });

    return executor != nullptr;
}

void RenderGraph::Execute(RenderContext *ctx)
{
    if (!Compile(ctx))
    {
        CT_LOG(Error, CT_TEXT("RendeGraph::Execute() failed to compile."));
        return;
    }

    CT_CHECK(executor);
    executor->Execute({
        .renderContext = ctx,
        .defaultResourceProps = defaultResourceProps,
        .resourceCache = executor->GetResourceCache().get(),
    });
}

void RenderGraph::OnResize(const FrameBuffer *targetFbo)
{
    const auto texture = targetFbo->GetColorTexture(0);
    CT_CHECK(texture != nullptr);

    defaultResourceProps.format = texture->GetResourceFormat();
    defaultResourceProps.width = targetFbo->GetWidth();
    defaultResourceProps.height = targetFbo->GetHeight();

    recompile = true;
}
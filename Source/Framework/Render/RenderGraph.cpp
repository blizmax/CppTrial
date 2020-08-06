#include "Render/RenderGraph.h"
#include "Render/RenderGraph/RenderGraphCompiler.h"
#include "Render/RenderGraph/RenderGraphExecutor.h"

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

void RenderGraph::SetScene(const SPtr<Scene> &inScene)
{
    if (scene == inScene)
        return;

    scene = inScene;
    //TODO notify all passes

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

    //TODO

    graph.RemoveNode(passIndex);
    passNameToIndex.Remove(name);

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

    //TODO Check edge not exists

    //TODO Check field in certain pass exists

    Edge edge = { srcPort, dstPort };
    graph.AddEdge(std::move(edge));

    recompile = true;
    return -1;
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
    //TODO
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

    //TODO
    return nullptr;
}

void RenderGraph::MarkOutput(const String &name)
{
    auto port = ParseNodePort(name);
    if (GetOutputIndex(port) == -1)
    {
        //TODO Check can be output
        outputs.Add(port);
        recompile = true;
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
    executor = RenderGraphCompiler::Compile(*this, ctx, dependencies);

    return executor != nullptr;
}
#include "Render/RenderGraph.h"

SPtr<RenderGraph> RenderGraph::Create()
{
    return Memory::MakeShared<RenderGraph>();
}

RenderGraph::RenderGraph()
{
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

    passIndex = graph.AddNode({name, pass});
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

int32 RenderGraph::AddEdge(const String &from, const String &to)
{
    //TODO
    return -1;
}

bool RenderGraph::RemoveEdge(const String &from, const String &to)
{
    //TODO
    return false;
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

SPtr<Resource> RenderGraph::GetOutput(const String &name)
{
    //TODO
    return nullptr;
}

bool RenderGraph::Compile()
{
    //TODO
    return false;
}
#include "Render/RenderGraph.h"

RenderGraph::RenderGraph()
{

}

int32 RenderGraph::GetPassIndex(const String &name) const
{
    auto ptr = passNameToIndex.TryGet(name);
    return ptr ? *ptr : -1;
}

int32 RenderGraph::AddPass(const String &name, const SPtr<RenderPass> &pass)
{
    if (GetPassIndex(name) != -1)
    {
        CT_LOG(Warning, CT_TEXT("Add pass failed, pass named {0} already exists"), name);
        return -1;
    }

    //TODO
    return -1;
}

bool RenderGraph::RemovePass(const String &name)
{
    //TODO
    return true;
}
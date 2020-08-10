#include "Render/RenderGraph/RenderGraphExecutor.h"

SPtr<RenderGraphExecutor> RenderGraphExecutor::Create()
{
    return Memory::MakeShared<RenderGraphExecutor>();
}

void RenderGraphExecutor::Execute(const RenderGraph::ExecuteContext &ctx)
{
    for (auto &e : executionList)
    {
        e.pass->Execute(ctx);
    }
}

void RenderGraphExecutor::SetInput(const String &name, const SPtr<Resource> &resource)
{
    resourceCache->RegisterExternalResource(name, resource);
}

SPtr<Resource> RenderGraphExecutor::GetResource(const String &name) const
{
    return resourceCache->GetResource(name);
}
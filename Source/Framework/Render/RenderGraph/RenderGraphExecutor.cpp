#include "Render/RenderGraph/RenderGraphExecutor.h"

SPtr<RenderGraphExecutor> RenderGraphExecutor::Create()
{
    return Memory::MakeShared<RenderGraphExecutor>();
}

void RenderGraphExecutor::Execute(const RenderGraph::ExecuteContext &ctx)
{
    //TODO

}
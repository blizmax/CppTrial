#include "Render/RenderGraph/RenderGraphExecutor.h"

SPtr<RenderGraphExecutor> RenderGraphExecutor::Create()
{
    return Memory::MakeShared<RenderGraphExecutor>();
}

void RenderGraphExecutor::Execute()
{
    //TODO
}
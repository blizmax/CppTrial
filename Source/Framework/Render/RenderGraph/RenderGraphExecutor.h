#pragma once

#include "Render/RenderGraph/RenderGraphResourceCache.h"

class RenderGraphExecutor
{
public:
    void Execute(const RenderGraph::ExecuteContext &ctx);

    static SPtr<RenderGraphExecutor> Create();

private:
    SPtr<RenderGraphResourceCache> resourceCache;
};
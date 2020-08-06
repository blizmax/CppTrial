#include "Render/RenderGraph/RenderGraphCompiler.h"
#include "Render/RenderGraph/RenderGraphExecutor.h"
#include "Render/RenderGraph.h"

RenderGraphCompiler::RenderGraphCompiler(RenderGraph &graph)
    : graph(graph)
{
}

SPtr<RenderGraphExecutor> RenderGraphCompiler::Compile(RenderGraph &graph, RenderContext *ctx, const Dependencies &deps)
{
    RenderGraphCompiler compiler(graph);

    auto resourceCache = RenderGraphResourceCache::Create();
    


    //TODO

    return nullptr;
}
#include "Render/RenderGraph/RenderGraphCompiler.h"
#include "Render/RenderGraph/RenderGraphExecutor.h"
#include "Render/RenderGraph.h"

RenderGraphCompiler::RenderGraphCompiler(RenderGraph &graph)
    : graph(graph)
{
}

SPtr<RenderGraphExecutor> RenderGraphCompiler::Compile(RenderGraph &graph, RenderContext *ctx)
{
    RenderGraphCompiler compiler(graph);


    //TODO

    return nullptr;
}
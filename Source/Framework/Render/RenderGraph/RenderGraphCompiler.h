#pragma once

#include "Render/RenderGraph/.Package.h"

class RenderGraphCompiler
{
public:
    static SPtr<RenderGraphExecutor> Compile(RenderGraph &graph, RenderContext *ctx);

private:
    RenderGraphCompiler(RenderGraph &graph);

private:
    RenderGraph &graph;    

};
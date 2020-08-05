#pragma once

#include "Render/RenderGraph/RenderPassReflection.h"
#include "Render/RenderGraph/RenderGraphResourceCache.h"

class RenderGraphCompiler
{
public:
    static SPtr<RenderGraphExecutor> Compile(RenderGraph &graph, RenderContext *ctx);

private:
    RenderGraphCompiler(RenderGraph &graph);

    void CompilePasses(RenderContext *ctx);

private:
    struct PassData
    {
        SPtr<RenderPass> pass;
        RenderPassReflection reflection;
    };

    RenderGraph &graph;    

};
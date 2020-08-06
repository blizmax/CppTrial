#pragma once

#include "Render/RenderGraph/RenderPassReflection.h"
#include "Render/RenderGraph/RenderGraphResourceCache.h"

class RenderGraphCompiler
{
public:
    struct Dependencies
    {
        RenderGraphResourceCache::DefaultProperties defaultResourceProps;
        HashMap<String, SPtr<Resource>> externalResources;
    };

    static SPtr<RenderGraphExecutor> Compile(RenderGraph &graph, RenderContext *ctx, const Dependencies &deps);

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
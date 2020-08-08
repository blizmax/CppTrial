#pragma once

#include "Render/RenderGraph/RenderGraphResourceCache.h"

class RenderGraphCompiler
{
public:
    using CompileContext = RenderGraph::CompileContext;
    static SPtr<RenderGraphExecutor> Compile(RenderGraph &graph, const CompileContext &ctx);

private:
    struct PassData
    {
        int32 nodeID;
        String name;
        SPtr<RenderPass> pass;
        RenderPassReflection reflection;
    };

    RenderGraphCompiler(RenderGraph &graph, const CompileContext &ctx);

    void ResolveExecutionOrder();
    void CompilePasses();
    bool InsertAutoPasses();
    bool ValidateGraph();
    void AllocateResources(RenderGraphResourceCache *resourceCache);

private:
    RenderGraph &graph;
    const CompileContext &ctx;
    Array<PassData> executionList;
};
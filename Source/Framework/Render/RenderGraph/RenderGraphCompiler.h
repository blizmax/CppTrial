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

    RenderGraphCompiler(RenderGraph &graph);

    void ResolveExecutionOrder(const CompileContext &ctx);
    void CompilePasses(const CompileContext &ctx);
    bool InsertAutoPasses();
    void ValidateGraph();

private:
    RenderGraph &graph;
    Array<PassData> executionList;
};
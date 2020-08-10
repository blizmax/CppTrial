#pragma once

#include "Render/RenderGraph/RenderGraphResourceCache.h"
#include "Render/RenderGraph/RenderGraphExecutor.h"

class RenderGraphCompiler
{
public:
    using CompileContext = RenderGraph::CompileContext;
    static SPtr<RenderGraphExecutor> Compile(RenderGraph &graph, const CompileContext &ctx);

private:
    RenderGraphCompiler(RenderGraph &graph, const CompileContext &ctx);

    void ResolveExecutionOrder();
    void CompilePasses();
    bool InsertAutoPasses();
    bool ValidateGraph();
    void AllocateResources(RenderGraphResourceCache *resourceCache);
    void RestoreCompilationChanges();

private:
    RenderGraph &graph;
    const CompileContext &ctx;
    Array<RenderGraphExecutor::ExecuteData> executionList;
};
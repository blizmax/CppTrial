#pragma once

#include "Render/RenderGraph/RenderGraphResourceCache.h"

class RenderGraphExecutor
{
public:
    void Execute(const RenderGraph::ExecuteContext &ctx);
    void SetInput(const String &name, const SPtr<Resource> &resource);
    SPtr<Resource> GetResource(const String &name) const;

    const SPtr<RenderGraphResourceCache> GetResourceCache() const
    {
        return resourceCache;
    }

    static SPtr<RenderGraphExecutor> Create();

private:
    friend class RenderGraphCompiler;

    struct ExecuteData
    {
        int32 nodeID;
        String name;
        SPtr<RenderPass> pass;
        RenderPassReflection reflection;
    };

    SPtr<RenderGraphResourceCache> resourceCache;
    Array<ExecuteData> executionList;
};
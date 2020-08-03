#pragma once

#include "Render/.Package.h"
#include "RenderCore/RenderContext.h"

class RenderGraph;

class RenderPass
{
public:
    virtual ~RenderPass() = default;

    virtual void Compile(RenderContext *ctx)
    {
    }

    virtual void Execute(RenderContext *ctx)
    {
    }

    void SetRenderGraph(RenderGraph *graph)
    {
        renderGraph = graph;
    }

    void SetName(const String &inName)
    {
        name = inName;
    }

    const String &GetName() const
    {
        return name;
    }

protected:
    String name;
    RenderGraph *renderGraph = nullptr;
};
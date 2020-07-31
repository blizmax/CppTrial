#pragma once

#include "Render/.Package.h"

class RenderGraph;

class RenderPass
{
public:
    virtual ~RenderPass() = default;

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
#pragma once

#include "RenderCore/GraphicsStateObject.h"
#include "RenderCore/FrameBuffer.h"
#include "RenderCore/StateGraph.h"

namespace RenderCore
{

class GraphicsState
{
public:
    ~GraphicsState() = default;

    void SetFrameBuffer(const SPtr<FrameBuffer> &fb);

    const SPtr<VertexArray> &GetVertexArray() const
    {
        return vertexArray;
    }

    const SPtr<FrameBuffer> &GetFrameBuffer() const
    {
        return frameBuffer;
    }

    void SetStencilRef(uint8 ref)
    {
        stencilRef = ref;
    }

    uint8 GetStencilRef() const
    {
        return stencilRef;
    }

    void SetViewport(uint32 index, const Viewport &vp, bool setScissors = true);

    const Viewport &GetViewport(uint32 index) const
    {
        return viewports[index];
    }

    const Array<Viewport> &GetViewports() const
    {
        return viewports;
    }

    static SPtr<GraphicsState> Create();

private:
    GraphicsState();

    SPtr<VertexArray> vertexArray;
    SPtr<FrameBuffer> frameBuffer;
    SPtr<RootSignature> rootSignature;

    uint8 stencilRef = 0;
    Array<Viewport> viewports;
    Array<Scissor> scissors;

    StateGraph<SPtr<GraphicsStateObject>> stateGraph;
};

}
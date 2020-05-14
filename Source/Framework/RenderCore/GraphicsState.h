#pragma once

#include "RenderCore/GraphicsStateObject.h"
#include "RenderCore/FrameBuffer.h"
#include "RenderCore/StateGraph.h"

namespace RenderCore
{

class GraphicsState
{
public:
    virtual ~GraphicsState() = default;

    static SPtr<GraphicsState> Create();

protected:
    GraphicsState();

    SPtr<VertexArray> vertexArray;
    SPtr<FrameBuffer> frameBuffer;
    SPtr<RootSignature> rootSignature;

    uint32 stencilRef = 0;
    Array<Viewport> viewports;
    Array<Scissor> scissors;
};

}
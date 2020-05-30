#pragma once

#include "RenderCore/RasterizationState.h"
#include "RenderCore/DepthStencilState.h"
#include "RenderCore/BlendState.h"
#include "RenderCore/VertexArray.h"
#include "RenderCore/RootSignature.h"
#include "RenderCore/FrameBuffer.h"

namespace RenderCore
{
struct GraphicsStateObjectDesc
{
    SPtr<VertexLayout> vertexLayout;
    SPtr<RasterizationState> rasterizationState;
    SPtr<DepthStencilState> depthStencilState;
    SPtr<BlendState> blendState;
    SPtr<RootSignature> rootSignature;

    FrameBufferDesc frameBufferDesc;
    Topology topology = Topology::TriangleList;
    uint32 sampleMask = UINT32_MAX;
};

class GraphicsStateObject
{
public:
    virtual ~GraphicsStateObject() = default;

    const GraphicsStateObjectDesc &GetDesc() const
    {
        return desc;
    }

    SPtr<GraphicsStateObject> Create(const GraphicsStateObjectDesc &desc);

protected:
    GraphicsStateObject(const GraphicsStateObjectDesc &desc) : desc(desc)
    {
    }

protected:
    GraphicsStateObjectDesc desc;
};
}
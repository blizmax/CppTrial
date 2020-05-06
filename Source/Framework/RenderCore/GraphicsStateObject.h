#pragma once

#include "RenderCore/Shader.h"
#include "RenderCore/RasterizerState.h"
#include "RenderCore/DepthStencilState.h"
#include "RenderCore/BlendState.h"

namespace RenderCore
{
struct GraphicsStateObjectDesc
{
    SPtr<RasterizationState> rasterizationState;
    SPtr<DepthStencilState> depthStencilState;
    SPtr<BlendState> blendState;
};

class GraphicsStateObject
{
public:
    GraphicsStateObject(const GraphicsStateObjectDesc &desc) : desc(desc)
    {
    }

    const GraphicsStateObjectDesc &GetDesc() const
    {
        return desc;
    }

    SPtr<GraphicsStateObject> Create(const GraphicsStateObjectDesc &desc)
    {
        return Memory::MakeShared<GraphicsStateObject>(desc);
    }
private:
    GraphicsStateObjectDesc desc;
};
}
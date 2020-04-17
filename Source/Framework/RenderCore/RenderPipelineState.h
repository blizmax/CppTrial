#pragma once

#include "RenderCore/BlendState.h"
#include "RenderCore/DepthStencilState.h"
#include "RenderCore/RasterizationState.h"
#include "RenderCore/Shader.h"

namespace RenderCore
{
struct RenderPipelineStateCreateParams
{
    SPtr<RasterizationState> rasterizationState;
    SPtr<BlendState> blendState;
    SPtr<DepthStencilState> depthStencilState;
    SPtr<Shader> shader;
};

class RenderPipelineState
{
public:
    static SPtr<RenderPipelineState> Create(const RenderPipelineStateCreateParams &params);
};
}
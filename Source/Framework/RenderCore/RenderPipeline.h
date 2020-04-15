#pragma once

#include "RenderCore/BlendState.h"
#include "RenderCore/DepthStencilState.h"
#include "RenderCore/RasterizationState.h"
#include "RenderCore/Shader.h"

namespace RenderCore
{
struct RenderPipelineCreateParams
{
    SPtr<RasterizationState> rasterizationState;
    SPtr<BlendState> blendState;
    SPtr<DepthStencilState> depthStencilState;
    SPtr<Shader> shader;
};

class RenderPipeline
{
public: 
    static SPtr<RenderPipeline> Create(const RenderPipelineCreateParams &params);
};
}
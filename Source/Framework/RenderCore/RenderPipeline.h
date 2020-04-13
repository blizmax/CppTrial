#pragma once

#include "RenderCore/RenderState.h"

namespace RenderCore
{
class RenderPipeline
{
public: 
    SPtr<BlendState> blendState;
    SPtr<DepthStencilState> depthStencilState;
};
}
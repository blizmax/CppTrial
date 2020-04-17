#pragma once

#include "RenderCore/RenderPipelineState.h"

namespace RenderCore
{
class RenderAPI
{
public:
    virtual ~RenderAPI() = default;

    virtual void Init() = 0;
    virtual void Destroy() = 0;

    //virtual void SetRenderPipeline(const SPtr<RenderPipelineState> &pipeline) = 0;
};
}
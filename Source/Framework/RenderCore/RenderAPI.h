#pragma once

#include "RenderCore/RenderPipeline.h"

namespace RenderCore
{
class RenderAPI
{
public:
    virtual ~RenderAPI() = default;

    virtual void Init() = 0;
    virtual void Destroy() = 0;

    virtual void SetRenderPipeline(const SPtr<RenderPipeline> &pipeline) = 0;
};
}
#pragma once

#include "RenderCore/ComputeContext.h"

namespace RenderCore
{
class RenderContext : public ComputeContext
{
public:
    virtual ~RenderContext() = default;

    static SPtr<RenderContext> Create();
};
}
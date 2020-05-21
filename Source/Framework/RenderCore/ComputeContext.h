#pragma once

#include "RenderCore/CopyContext.h"

namespace RenderCore
{

class ComputeContext : public CopyContext
{
public:
    virtual ~ComputeContext() = default;

    static SPtr<ComputeContext> Create();
};

}
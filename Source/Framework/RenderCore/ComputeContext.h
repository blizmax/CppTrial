#pragma once

#include "RenderCore/CopyContext.h"

namespace RenderCore
{

class ComputeContext : public CopyContext
{
public:
    virtual ~ComputeContext() = default;

    virtual void Flush(bool wait = false) = 0;

    //virtual void Dispatch();

    static SPtr<ComputeContext> Create(const SPtr<GpuQueue> &queue);
};

}
#pragma once

#include "RenderCore/QueryPool.h"

class GpuTimer
{
public:
    virtual ~GpuTimer() = default;

    virtual void Begin() = 0;
    virtual void End() = 0;

    static SPtr<GpuTimer> Create();
};
#pragma once

#include "RenderCore/GpuQueue.h"

namespace RenderCore
{

class GpuFence
{
public:
    virtual ~GpuFence() = default;

    virtual uint64 GpuSignal(GpuQueue *queue) = 0;
    virtual void SyncGpu(GpuQueue *queue) = 0;
    virtual void SyncCpu() = 0;

    virtual uint64 GetGpuValue() = 0;
    virtual uint64 GetCpuValue() = 0;

    static SPtr<GpuFence> Create();
};

}
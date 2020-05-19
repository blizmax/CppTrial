#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
class GpuQueue
{
public:
    virtual ~GpuQueue() = default;

    virtual GpuQueueType GetQueueType() const = 0;

    static SPtr<GpuQueue> Create(GpuQueueType queueType, void *handle);
};

}
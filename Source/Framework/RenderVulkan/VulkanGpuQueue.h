#pragma once

#include "RenderCore/GpuQueue.h"
#include "RenderVulkan/.Package.h"

class VulkanGpuQueue : public GpuQueue
{
public:
    VulkanGpuQueue(GpuQueueType queueType, void *handle);

    virtual GpuQueueType GetQueueType() const override
    {
        return queueType;
    }

    VkQueue GetHandle() const
    {
        return queue;
    }

private:
    VkQueue queue = VK_NULL_HANDLE;
    GpuQueueType queueType;
};
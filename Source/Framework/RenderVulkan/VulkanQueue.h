#pragma once

#include "RenderVulkan/.Package.h"

namespace RenderCore
{

struct VulkanQueueCreateParams
{
    GpuQueueType queueType = GpuQueueType::Graphics;
    VkQueue queue = VK_NULL_HANDLE;
};

class VulkanQueue
{
public:
    VulkanQueue(const VulkanQueueCreateParams &params);

    void WaitIdle() const;

    VkQueue GetHandle() const
    {
        return queue;
    }

    GpuQueueType GetQueueType() const
    {
        return queueType;
    }

    static SPtr<VulkanQueue> Create(const VulkanQueueCreateParams &params);

private:
    VkQueue queue = VK_NULL_HANDLE;
    GpuQueueType queueType;
};    
}
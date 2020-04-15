#pragma once

#include "RenderVulkan/.Package.h"

namespace RenderCore
{
class VulkanQueue
{
public:
    VulkanQueue(GpuQueueType queueType, VkQueue queue);

    static SPtr<VulkanQueue> Create(GpuQueueType queueType, VkQueue queue);

private:
    VkQueue queue = VK_NULL_HANDLE;
    GpuQueueType queueType;
};    
}
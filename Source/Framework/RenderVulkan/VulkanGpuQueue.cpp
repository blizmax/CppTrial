#include "RenderVulkan/VulkanGpuQueue.h"

SPtr<GpuQueue> GpuQueue::Create(GpuQueueType queueType, void *handle)
{
    return Memory::MakeShared<VulkanGpuQueue>(queueType, handle);
}

VulkanGpuQueue::VulkanGpuQueue(GpuQueueType queueType, void *handle)
    : queueType(queueType), queue((VkQueue)handle)
{
}
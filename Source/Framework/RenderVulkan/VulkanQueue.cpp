#include "RenderVulkan/VulkanQueue.h"

namespace RenderCore
{

SPtr<VulkanQueue> VulkanQueue::Create(GpuQueueType queueType, VkQueue queue)
{
    return Memory::MakeShared<VulkanQueue>(queueType, queue);
}

VulkanQueue::VulkanQueue(GpuQueueType queueType, VkQueue queue)
    : queueType(queueType), queue(queue)
{
    
}

}
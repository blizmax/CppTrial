#include "RenderVulkan/VulkanQueue.h"

namespace RenderCore
{

SPtr<VulkanQueue> VulkanQueue::Create(const VulkanQueueCreateParams &params)
{
    return Memory::MakeShared<VulkanQueue>(params);
}

VulkanQueue::VulkanQueue(const VulkanQueueCreateParams &params)
    : queueType(params.queueType), queue(params.queue)
{
}

void VulkanQueue::WaitIdle() const
{
    if (vkQueueWaitIdle(queue) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Queue wait idle failed.");
}

}
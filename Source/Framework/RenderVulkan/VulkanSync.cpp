#include "RenderVulkan/VulkanSync.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
SPtr<VulkanSemaphore> VulkanSemaphore::Create()
{
    return Memory::MakeShared<VulkanSemaphore>();
}

VulkanSemaphore::VulkanSemaphore()
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();

    VkSemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
    semaphoreInfo.flags = 0;

    if(vkCreateSemaphore(device, &semaphoreInfo, gVulkanAlloc, &semaphore))
        CT_EXCEPTION(RenderCore, "Create semaphore failed.");
}

void VulkanSemaphore::Destroy()
{
    if(semaphore != VK_NULL_HANDLE)
    {
        auto device = VulkanContext::Get().GetLogicalDeviceHandle();
        vkDestroySemaphore(device, semaphore, gVulkanAlloc);
        semaphore = VK_NULL_HANDLE;
    }
}

SPtr<VulkanFence> VulkanFence::Create()
{
    return Memory::MakeShared<VulkanFence>();
}

VulkanFence::VulkanFence()
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = nullptr;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if(vkCreateFence(device, &fenceInfo, gVulkanAlloc, &fence))
        CT_EXCEPTION(RenderCore, "Create fence failed.");
}

void VulkanFence::Destroy()
{
    if(fence != VK_NULL_HANDLE)
    {
        auto device = VulkanContext::Get().GetLogicalDeviceHandle();
        vkDestroyFence(device, fence, gVulkanAlloc);
        fence = VK_NULL_HANDLE;
    }
}

void VulkanFence::Wait()
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();
    vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
}

void VulkanFence::Reset()
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();
    vkResetFences(device, 1, &fence);
}

}

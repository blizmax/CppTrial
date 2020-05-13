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
    VkSemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if(vkCreateSemaphore(gVulkanContext->GetLogicalDeviceHandle(), &semaphoreInfo, gVulkanAlloc, &semaphore))
        CT_EXCEPTION(RenderCore, "Create semaphore failed.");
}

void VulkanSemaphore::Destroy()
{
    if(semaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(gVulkanContext->GetLogicalDeviceHandle(), semaphore, gVulkanAlloc);
        semaphore = VK_NULL_HANDLE;
    }
}

SPtr<VulkanFence> VulkanFence::Create()
{
    return Memory::MakeShared<VulkanFence>();
}

VulkanFence::VulkanFence()
{
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if(vkCreateFence(gVulkanContext->GetLogicalDeviceHandle(), &fenceInfo, gVulkanAlloc, &fence))
        CT_EXCEPTION(RenderCore, "Create fence failed.");
}

void VulkanFence::Destroy()
{
    if(fence != VK_NULL_HANDLE)
    {
        vkDestroyFence(gVulkanContext->GetLogicalDeviceHandle(), fence, gVulkanAlloc);
        fence = VK_NULL_HANDLE;
    }
}

void VulkanFence::Wait()
{
    vkWaitForFences(gVulkanContext->GetLogicalDeviceHandle(), 1, &fence, VK_TRUE, UINT64_MAX);
}

void VulkanFence::Reset()
{
    vkResetFences(gVulkanContext->GetLogicalDeviceHandle(), 1, &fence);
}

}

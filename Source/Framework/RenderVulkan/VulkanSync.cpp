#include "RenderVulkan/VulkanSync.h"
#include "RenderVulkan/VulkanDevice.h"

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

    if (vkCreateSemaphore(gVulkanDevice->GetLogicalDeviceHandle(), &semaphoreInfo, gVulkanAlloc, &semaphore) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create semaphore failed.");
}

void VulkanSemaphore::Destroy()
{
    if(semaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(gVulkanDevice->GetLogicalDeviceHandle(), semaphore, gVulkanAlloc);
        semaphore = VK_NULL_HANDLE;
    }
}

SPtr<VulkanFence> VulkanFence::Create(bool signaled)
{
    return Memory::MakeShared<VulkanFence>(signaled);
}

VulkanFence::VulkanFence(bool signaled)
{
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    if (signaled)
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if(vkCreateFence(gVulkanDevice->GetLogicalDeviceHandle(), &fenceInfo, gVulkanAlloc, &fence) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create fence failed.");
}

void VulkanFence::Destroy()
{
    if(fence != VK_NULL_HANDLE)
    {
        vkDestroyFence(gVulkanDevice->GetLogicalDeviceHandle(), fence, gVulkanAlloc);
        fence = VK_NULL_HANDLE;
    }
}

void VulkanFence::Wait()
{
    vkWaitForFences(gVulkanDevice->GetLogicalDeviceHandle(), 1, &fence, VK_TRUE, UINT64_MAX);
}

void VulkanFence::Reset()
{
    vkResetFences(gVulkanDevice->GetLogicalDeviceHandle(), 1, &fence);
}

bool VulkanFence::IsSignaled() const
{
    return vkGetFenceStatus(gVulkanDevice->GetLogicalDeviceHandle(), fence) == VK_SUCCESS;
}

}

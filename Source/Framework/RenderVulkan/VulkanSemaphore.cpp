#include "RenderVulkan/VulkanSemaphore.h"
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

}

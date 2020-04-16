#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{
class VulkanSemaphore : public IVulkanResource
{
public:
    VulkanSemaphore();

    virtual void Destroy() override;

    VkSemaphore GetHandle() const
    {
        return semaphore;
    }

    static SPtr<VulkanSemaphore> Create();

private:
    VkSemaphore semaphore = VK_NULL_HANDLE;
};
}
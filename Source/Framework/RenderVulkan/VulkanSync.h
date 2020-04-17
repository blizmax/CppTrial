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

class VulkanFence : public IVulkanResource
{
public:
    VulkanFence();

    virtual void Destroy() override;

    VkFence GetHandle() const
    {
        return fence;
    }

    static SPtr<VulkanFence> Create();

private:
    VkFence fence = VK_NULL_HANDLE;
};
}
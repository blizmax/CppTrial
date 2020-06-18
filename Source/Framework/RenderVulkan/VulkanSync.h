#pragma once

#include "RenderVulkan/VulkanMemory.h"

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
    VulkanFence(bool signaled);

    virtual void Destroy() override;

    void Wait();
    void Reset();
    bool IsSignaled() const;

    VkFence GetHandle() const
    {
        return fence;
    }

    static SPtr<VulkanFence> Create(bool signaled = false);

private:
    VkFence fence = VK_NULL_HANDLE;
};
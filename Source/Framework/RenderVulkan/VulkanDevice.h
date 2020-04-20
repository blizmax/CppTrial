#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{
class VulkanDevice : public IVulkanResource
{
public:
    struct QueueData
    {
        uint32 familyIndex;
        Array<SPtr<VulkanQueue>> queues;
    };

    VulkanDevice(VkPhysicalDevice device);
    ~VulkanDevice();

    virtual void Destroy() override;

    auto GetGraphicsQueueFamilyIndex() const
    {
        return graphicsQueueData.familyIndex;
    }

    SPtr<VulkanQueue> GetGraphicsQueue() const
    {
        return graphicsQueueData.queues[0];
    }

    VkPhysicalDevice GetPhysicalDeviceHandle() const
    {
        return physicalDevice;
    }

    VkDevice GetLogicalDeviceHandle() const
    {
        return logicalDevice;
    }

    VmaAllocator GetAllocator() const
    {
        return allocator;
    }

    static SPtr<VulkanDevice> Create(VkPhysicalDevice device);

private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VmaAllocator allocator = VK_NULL_HANDLE;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    QueueData graphicsQueueData;
};
}
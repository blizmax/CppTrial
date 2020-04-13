#pragma once

#include "RenderVulkan/.Package.h"

namespace RenderCore
{
class VulkanDevice
{
public:
    VulkanDevice(VkPhysicalDevice device);
    ~VulkanDevice();

    auto GetGraphicsQueueFamilyIndex() const
    {
        return graphicsQueueData.familyIndex;
    }

    VkPhysicalDevice GetPhysicalDeviceHandle() const
    {
        return physicalDevice;
    }

    VkDevice GetLogicalDeviceHandle() const
    {
        return logicalDevice;
    }

    static UPtr<VulkanDevice> Create(VkPhysicalDevice device);

private:
    struct QueueData
    {
        uint32 familyIndex;
        Array<SPtr<VulkanQueue>> queues;
    };

    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    QueueData graphicsQueueData;
};
}
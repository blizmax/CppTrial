#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{
class VulkanDevice : public IVulkanResource
{
public:
    struct QueueData
    {
        uint32 familyIndex = UINT32_MAX;
        Array<SPtr<VulkanQueue>> queues;

        QueueData() = default;
        QueueData(uint32 index) : familyIndex(index)
        {
        }
    };

    VulkanDevice(VkPhysicalDevice device);
    ~VulkanDevice();

    virtual void Destroy() override;

    void WaitIdle();

    auto GetGraphicsQueueFamilyIndex() const
    {
        return queueDatas[(int32)GpuQueueType::Graphics].familyIndex;
    }

    SPtr<VulkanQueue> GetGraphicsQueue() const
    {
        return queueDatas[(int32)GpuQueueType::Graphics].queues[0];
    }

    VkPhysicalDevice GetPhysicalDeviceHandle() const
    {
        return physicalDevice;
    }

    VkDevice GetLogicalDeviceHandle() const
    {
        return logicalDevice;
    }

    static SPtr<VulkanDevice> Create(VkPhysicalDevice device);

private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    QueueData queueDatas[(int32)GpuQueueType::Count];
};
}
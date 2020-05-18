#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/Device.h"

namespace RenderCore
{
class VulkanDevice : public Device
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

    VulkanDevice(RenderWindow *window, const DeviceDesc &desc);
    virtual ~VulkanDevice();

    // void WaitIdle()
    // {
    //     vkDeviceWaitIdle(logicalDevice);
    // }

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

    VkSurfaceKHR GetSurfaceHandle()
    {
        return surface;
    }

    VkSwapchainKHR GetSwapChainHandle()
    {
        return swapChain;
    }

    VmaAllocator GetVmaAllocator()
    {
        return allocator;
    }

    const VkPhysicalDeviceProperties &GetDeviceProperties() const
    {
        return properties;
    }

    const VkPhysicalDeviceFeatures &GetDeviceFeatures() const
    {
        return features;
    }

    const VkPhysicalDeviceMemoryProperties &GetDeviceMemoryProperties() const
    {
        return memoryProperties;
    }

    const VkPhysicalDeviceLimits &GetDeviceLimits() const
    {
        return properties.limits;
    }

    virtual void ResizeSwapChain(uint32 width, uint32 height) override;
    virtual void Present() override;
    virtual void FlushAndSync() override;

private:
    void InitPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSurface();
    void CreateSwapChain();
    void CreateVmaAllocator();

    void UpdateBackBuffers(uint32 width, uint32 height, ResourceFormat colorFormat, ResourceFormat depthFormat);
    uint32 GetCurrentBackBufferIndex();

private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VmaAllocator allocator = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;

    Array<SPtr<VulkanFence>> presentFences;
    uint32 curPresentFenceIndex = 0;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    QueueData queueDatas[(int32)GpuQueueType::Count];
};
}
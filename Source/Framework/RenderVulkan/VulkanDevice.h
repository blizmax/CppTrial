#pragma once

#include "RenderCore/Device.h"
#include "RenderCore/GpuQueue.h"
#include "RenderVulkan/VulkanMemory.h"
#include "RenderVulkan/VulkanSync.h"

class VulkanDevice : public Device
{
public:
    struct QueueData
    {
        int32 familyIndex = -1;
        Array<SPtr<GpuQueue>> queues;

        QueueData() = default;
        QueueData(int32 index)
            : familyIndex(index)
        {
        }
    };

    VulkanDevice(RenderWindow *window, const DeviceDesc &desc);
    virtual ~VulkanDevice();

    int32 GetQueueFamilyIndex(GpuQueueType queueType) const
    {
        return queueDatas[(int32)queueType].familyIndex;
    }

    SPtr<GpuQueue> GetQueue(GpuQueueType queueType, int32 index = 0) const
    {
        auto &queues = queueDatas[(int32)queueType].queues;
        return queues[index];
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

    virtual void ResizeSwapChain(int32 width, int32 height) override;
    virtual void Present() override;
    virtual void FlushAndSync() override;

private:
    void InitPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSurface();
    void CreateSwapChain();
    void CreateVmaAllocator();

    void UpdateBackBuffers(int32 width, int32 height, ResourceFormat colorFormat, ResourceFormat depthFormat);
    int32 GetCurrentBackBufferIndex();

private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VmaAllocator allocator = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;

    Array<SPtr<VulkanFence>> presentFences;
    int32 curPresentFenceIndex = 0;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    QueueData queueDatas[(int32)GpuQueueType::Count];
};
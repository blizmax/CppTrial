#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/VulkanQueue.h"

namespace RenderCore
{
SPtr<VulkanDevice> VulkanDevice::Create(VkPhysicalDevice device)
{
    return Memory::MakeShared<VulkanDevice>(device);
}

VulkanDevice::VulkanDevice(VkPhysicalDevice device) : physicalDevice(device)
{
    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceFeatures(device, &features);
    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

    uint32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    Array<VkQueueFamilyProperties> queueFamilies;
    queueFamilies.AppendUninitialized(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.GetData());

    Array<VkDeviceQueueCreateInfo> queueCreateInfos;
    auto DetermineQueue = [this](Array<VkDeviceQueueCreateInfo> &arr, GpuQueueType q, uint32 i) {
        if(queueDatas[(int32)q].familyIndex != UINT32_MAX)
            return false;

        queueDatas[(int32)q].familyIndex = i;
        queueDatas[(int32)q].queues.Add(nullptr);
        const float priority = 1.0f;
        VkDeviceQueueCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.queueFamilyIndex = i;
        info.queueCount = 1;
        info.pQueuePriorities = &priority;
        arr.Add(info);
        return true;
    };
    for (int32 i = 0; i < queueFamilies.Count(); ++i)
    {      
        if(queueFamilies[i].queueCount == 0)
            continue;
        auto flags = queueFamilies[i].queueFlags;
        if((flags & VK_QUEUE_GRAPHICS_BIT) != 0 && DetermineQueue(queueCreateInfos, GpuQueueType::Graphics, i))
            continue;
        if((flags & VK_QUEUE_COMPUTE_BIT) != 0 && DetermineQueue(queueCreateInfos, GpuQueueType::Compute, i))
            continue;
        if((flags & VK_QUEUE_TRANSFER_BIT) != 0 && DetermineQueue(queueCreateInfos, GpuQueueType::Transfer, i))
            continue;
    }

    Array<const char8 *> extensions;
    extensions.Add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    uint32 extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    Array<VkExtensionProperties> availableExtensions;
    availableExtensions.AppendUninitialized(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.GetData());
    for(const auto &e : extensions)
    {
        bool match = false;
        for(const auto &p : availableExtensions)
        {
            if(std::strcmp(p.extensionName, e) == 0)
            {
                match = true;
                break;
            }
        }
        if(!match)
            CT_EXCEPTION(RenderCore, "Device extension unsupported.");
    }

    // Create logic device
    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = queueCreateInfos.Count();
    deviceInfo.pQueueCreateInfos = queueCreateInfos.GetData();
    deviceInfo.pEnabledFeatures = &features;
    deviceInfo.enabledExtensionCount = extensions.Count();
    deviceInfo.ppEnabledExtensionNames = extensions.GetData();
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = nullptr;

    if (vkCreateDevice(device, &deviceInfo, gVulkanAlloc, &logicalDevice) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create logic device failed.");

    for (int32 i = 0; i < (int32)GpuQueueType::Count; ++i)
    {
        auto &queueData = queueDatas[i];
        for(int32 i = 0; i < queueData.queues.Count(); ++i)
        {
            VkQueue queue;
            vkGetDeviceQueue(logicalDevice, queueData.familyIndex, i, &queue);
            VulkanQueueCreateParams params = {GpuQueueType::Graphics, queue};
            queueData.queues[i] = VulkanQueue::Create(params);
        }
    }
}

VulkanDevice::~VulkanDevice()
{
}

void VulkanDevice::Destroy()
{
    if(logicalDevice != VK_NULL_HANDLE)
    {
        vkDestroyDevice(logicalDevice, gVulkanAlloc);
        logicalDevice = VK_NULL_HANDLE;
    }
}

void VulkanDevice::WaitIdle()
{
    vkDeviceWaitIdle(logicalDevice);
}

}
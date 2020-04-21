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

    float priority = 1.0f;
    Array<VkDeviceQueueCreateInfo> queueCreateInfos;

    auto AppendQueueCreateInfo = [](Array<VkDeviceQueueCreateInfo> &arr, uint32 i, float p) {
        VkDeviceQueueCreateInfo info;
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.pNext = nullptr;
        info.flags = 0;
        info.queueFamilyIndex = i;
        info.queueCount = 1;
        info.pQueuePriorities = &p;
        arr.Add(info);
    };

    for (int32 i = 0; i < queueFamilies.Count(); ++i)
    {
        if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            //create one queue now
            float priority = 1.0f;
            graphicsQueueData.familyIndex = i;
            graphicsQueueData.queues.Add(nullptr);

            AppendQueueCreateInfo(queueCreateInfos, i, priority);
            break;
        }
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
    VkDeviceCreateInfo deviceInfo;
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = nullptr;
    deviceInfo.flags = 0;
    deviceInfo.queueCreateInfoCount = queueCreateInfos.Count();
    deviceInfo.pQueueCreateInfos = queueCreateInfos.GetData();
    deviceInfo.pEnabledFeatures = &features;
    deviceInfo.enabledExtensionCount = extensions.Count();
    deviceInfo.ppEnabledExtensionNames = extensions.GetData();
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = nullptr;

    if (vkCreateDevice(device, &deviceInfo, gVulkanAlloc, &logicalDevice) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create logic device failed.");

    for (int32 i = 0; i < graphicsQueueData.queues.Count(); ++i)
    {
        VkQueue queue;
        vkGetDeviceQueue(logicalDevice, graphicsQueueData.familyIndex, i, &queue);
        VulkanQueueCreateParams params = {GpuQueueType::Graphics, queue};
        graphicsQueueData.queues[i] = VulkanQueue::Create(params);
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
}
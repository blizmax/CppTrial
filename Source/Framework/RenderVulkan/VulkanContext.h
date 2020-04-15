#pragma once

#include "RenderVulkan/VulkanDevice.h"

namespace RenderCore
{
class VulkanContext
{
public:
    void Init();
    void Destroy();

    VkInstance GetInstanceHandle() const
    {
        return instance;
    }

    VkPhysicalDevice GetPhysicalDeviceHandle() const
    {
        return device->GetPhysicalDeviceHandle();
    }

    VkDevice GetLogicalDeviceHandle() const
    {
        return device->GetLogicalDeviceHandle();
    }

    SPtr<VulkanDevice> GetDevice() const
    {
        return device;
    }

    SPtr<VulkanRenderPass> GetRenderPass() const
    {
        //TODO
        return nullptr;
    }

    static VulkanContext &Get()
    {
        static VulkanContext instance;
        return instance;
    }

private:
    VulkanContext() = default;

    void CreateInstance();
    void CreateDebugger();
    void CreateDevice();

private:
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    SPtr<VulkanDevice> device;

    bool enableValidationLayers = false;
};
}
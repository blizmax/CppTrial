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

    const SPtr<VulkanDevice> &GetDevice() const
    {
        return device;
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
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

    SPtr<VulkanFrameBuffer> GetFrameBuffer() const
    {
        return frameBuffer; //TODO
    }

    SPtr<VulkanRenderPipeline> GetRenderPipeline() const
    {
        return pipeline; //TODO
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
    SPtr<VulkanFrameBuffer> frameBuffer;
    SPtr<VulkanRenderPipeline> pipeline;

    bool enableValidationLayers = false;
};
}
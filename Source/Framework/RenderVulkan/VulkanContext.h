#pragma once

#include "RenderVulkan/VulkanDevice.h"

namespace RenderCore
{
class VulkanContext
{
public:
    struct FrameData
    {
        SPtr<VulkanSemaphore> renderFinishedSemaphore;
        SPtr<VulkanSemaphore> swapChainImageSemaphore;
        SPtr<VulkanFence> fence;
    };

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
        return frameBuffer;
    }

    void SetFrameBuffer(const SPtr<VulkanFrameBuffer> &buffer)
    {
        frameBuffer = buffer;
    }

    SPtr<VulkanRenderPipeline> GetRenderPipeline() const
    {
        return renderPipeline;
    }

    void SetRenderPipeline(const SPtr<VulkanRenderPipeline> &pipeline)
    {
        renderPipeline = pipeline;
    }

    void Present();

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
    void CreateFrameDatas();
    void DestroyFrameDatas();

private:
    bool enableValidationLayers = false;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface; //TODO

    SPtr<VulkanDevice> device;
    SPtr<VulkanSwapChain> swapChain; //TODO

    SPtr<VulkanFrameBuffer> frameBuffer;
    SPtr<VulkanRenderPipeline> renderPipeline;

    FrameData frames[VULKAN_FRAME_NUM];
    int32 frameIndex = 0;
};
}
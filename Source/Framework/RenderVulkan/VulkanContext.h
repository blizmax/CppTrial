#pragma once

#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/VulkanSwapChain.h"
#include "RenderVulkan/VulkanShader.h"

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

    VkInstance GetInstanceHandle()
    {
        return instance;
    }

    VkPhysicalDevice GetPhysicalDeviceHandle()
    {
        return device->GetPhysicalDeviceHandle();
    }

    VkDevice GetLogicalDeviceHandle()
    {
        return device->GetLogicalDeviceHandle();
    }

    SPtr<VulkanDevice> GetDevice()
    {
        return device;
    }

    VkSurfaceKHR GetSurfaceKHR()
    {
        return surface;
    }

    void SetSurfaceKHR(VkSurfaceKHR surfaceKHR)
    {
        surface = surfaceKHR;
    }

    SPtr<VulkanSwapChain> GetSwapChain()
    {
        return swapChain;
    }

    SPtr<VulkanFrameBuffer> GetFrameBuffer()
    {
        return frameBuffer;
    }

    void SetFrameBuffer(const SPtr<VulkanFrameBuffer> &buffer)
    {
        frameBuffer = buffer;
    }

    SPtr<VulkanRenderPipelineState> GetRenderPipelineState()
    {
        return renderPipelineState;
    }

    void SetRenderPipelineState(const SPtr<VulkanRenderPipelineState> &state)
    {
        renderPipelineState = state;
    }

    FrameData &GetCurrentFrameData()
    {
        return frames[currentFrameIndex];
    }

    SPtr<VulkanCommandPool> GetRenderCommandPool()
    {
        return renderCommandPool;
    }

    VulkanResourceRegistry<VulkanShader> &GetShaderRegistry()
    {
        return shaderRegistry;
    }

    void Init();
    void Destroy();
    void RecreateSwapChain(const VulkanSwapChainCreateParams &params);
    void Submit(const SPtr<VulkanCommandBuffer> &buffer);
    void Present();

    SPtr<VulkanRenderPipeline> GetRenderPipeline();

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
    void CreateCommandPools();
    void DestroyFrameDatas();
    void DestroyCommandPools();

private:
    bool enableValidationLayers = false;
    VkInstance instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    SPtr<VulkanDevice> device;
    SPtr<VulkanSwapChain> swapChain;

    SPtr<VulkanFrameBuffer> frameBuffer;
    SPtr<VulkanRenderPipelineState> renderPipelineState;
    Array<SPtr<VulkanRenderPipeline>> renderPipelines;

    SPtr<VulkanCommandPool> renderCommandPool;

    VulkanResourceRegistry<VulkanShader> shaderRegistry;

    FrameData frames[VULKAN_FRAME_NUM];
    uint32 currentFrameIndex = 0;
};
}
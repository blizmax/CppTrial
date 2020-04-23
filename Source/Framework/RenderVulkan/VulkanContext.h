#pragma once

#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/VulkanSwapChain.h"
#include "RenderVulkan/VulkanShader.h"
#include "RenderVulkan/VulkanVertexBuffer.h"
#include "RenderVulkan/VulkanVertexLayout.h"
#include "RenderVulkan/VulkanIndexBuffer.h"

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

    struct StateData
    {
        SPtr<VulkanRenderPipelineState> renderPipelineState;
        SPtr<VulkanVertexLayout> vertexLayout;
        SPtr<VulkanFrameBuffer> frameBuffer;

        bool IsValid() const
        {
            return renderPipelineState != nullptr &&
                vertexLayout != nullptr &&
                frameBuffer != nullptr;
        }
    };

    struct RenderPipelineStorage
    {
        SPtr<VulkanRenderPipeline> renderPipeline;

        WPtr<VulkanRenderPipelineState> renderPipelineState;
        WPtr<VulkanVertexLayout> vertexLayout;
        WPtr<VulkanRenderPass> renderPass;
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

    VmaAllocator GetVmaAllocator()
    {
        return allocator;
    }

    void SetSurfaceKHR(VkSurfaceKHR surfaceKHR)
    {
        surface = surfaceKHR;
    }

    SPtr<VulkanSwapChain> GetSwapChain()
    {
        return swapChain;
    }

    VulkanShaderCompiler *GetShaderCompiler()
    {
        return shaderCompiler;
    }

    SPtr<VulkanFrameBuffer> GetCurrentFrameBuffer()
    {
        return currentState.frameBuffer;
    }

    void SetCurrentFrameBuffer(const SPtr<VulkanFrameBuffer> &buffer)
    {
        currentState.frameBuffer = buffer;
    }

    SPtr<VulkanRenderPipelineState> GetCurrentRenderPipelineState()
    {
        return currentState.renderPipelineState;
    }

    void SetCurrentRenderPipelineState(const SPtr<VulkanRenderPipelineState> &state)
    {
        currentState.renderPipelineState = state;
    }

    SPtr<VulkanVertexLayout> GetCurrentVertexLayout()
    {
        return currentState.vertexLayout;
    }

    void SetCurrentVertexLayout(const SPtr<VulkanVertexLayout> &layout)
    {
        currentState.vertexLayout = layout;
    }

    StateData &GetCurrentStateData()
    {
        return currentState;
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

    VulkanResourceRegistry<VulkanVertexBuffer> &GetVertexBufferRegistry()
    {
        return vertexBufferRegistry;
    }

    VulkanResourceRegistry<VulkanIndexBuffer> &GetIndexBufferRegistry()
    {
        return indexBufferRegistry;
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
    void CreateDevice();

    void CreateDebugger();
    void DestroyDebugger();
    void CreateShaderCompiler();
    void DestroyShaderCompiler();
    void CreateVmaAllocator();
    void DestroyVmaAllocator();
    void CreateFrameDatas();
    void DestroyFrameDatas();
    void CreateCommandPools();
    void DestroyCommandPools();

private:
    bool enableValidationLayers = false;
    VkInstance instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VmaAllocator allocator = VK_NULL_HANDLE;

    VulkanShaderCompiler *shaderCompiler = nullptr;

    SPtr<VulkanDevice> device;
    SPtr<VulkanSwapChain> swapChain;
    SPtr<VulkanCommandPool> renderCommandPool;

    Array<RenderPipelineStorage> renderPipelines;
    VulkanResourceRegistry<VulkanShader> shaderRegistry;
    VulkanResourceRegistry<VulkanVertexBuffer> vertexBufferRegistry;
    VulkanResourceRegistry<VulkanIndexBuffer> indexBufferRegistry;

    FrameData frames[VULKAN_FRAME_NUM];
    uint32 currentFrameIndex = 0;

    StateData currentState;
};
}
// #pragma once

// #include "RenderCore/Context.h"
// #include "RenderVulkan/VulkanDevice.h"
// #include "RenderVulkan/VulkanShader.h"

// namespace RenderCore
// {
// class VulkanContext : public Context
// {
// public:
//     struct FrameData
//     {
//         SPtr<VulkanSemaphore> renderFinishedSemaphore;
//         SPtr<VulkanSemaphore> swapChainImageSemaphore;
//         SPtr<VulkanFence> fence;
//     };

//     struct StateData
//     {
//         SPtr<VulkanRenderPipelineState> renderPipelineState;
//         SPtr<VulkanVertexLayout> vertexLayout;
//         SPtr<VulkanFrameBuffer> frameBuffer;

//         bool IsValid() const
//         {
//             return renderPipelineState != nullptr &&
//                 vertexLayout != nullptr &&
//                 frameBuffer != nullptr;
//         }
//     };

//     struct RenderPipelineStorage
//     {
//         SPtr<VulkanRenderPipeline> renderPipeline;

//         WPtr<VulkanRenderPipelineState> renderPipelineState;
//         WPtr<VulkanVertexLayout> vertexLayout;
//         WPtr<VulkanRenderPass> renderPass;
//     };

//     VkInstance GetInstanceHandle()
//     {
//         return instance;
//     }

//     // VkPhysicalDevice GetPhysicalDeviceHandle()
//     // {
//     //     return device->GetPhysicalDeviceHandle();
//     // }

//     // VkDevice GetLogicalDeviceHandle()
//     // {
//     //     return device->GetLogicalDeviceHandle();
//     // }

//     // SPtr<VulkanDevice> GetDevice()
//     // {
//     //     return device;
//     // }

//     // VkSurfaceKHR GetSurfaceHandle()
//     // {
//     //     return surface;
//     // }

//     // VkSwapchainKHR GetSwapChainHandle()
//     // {
//     //     return swapChain;
//     // }

//     // VmaAllocator GetVmaAllocator()
//     // {
//     //     return allocator;
//     // }

//     VulkanShaderCompiler *GetShaderCompiler()
//     {
//         return shaderCompiler;
//     }

//     SPtr<VulkanFrameBuffer> GetCurrentFrameBuffer()
//     {
//         return currentState.frameBuffer;
//     }

//     void SetCurrentFrameBuffer(const SPtr<VulkanFrameBuffer> &buffer)
//     {
//         currentState.frameBuffer = buffer;
//     }

//     SPtr<VulkanRenderPipelineState> GetCurrentRenderPipelineState()
//     {
//         return currentState.renderPipelineState;
//     }

//     void SetCurrentRenderPipelineState(const SPtr<VulkanRenderPipelineState> &state)
//     {
//         currentState.renderPipelineState = state;
//     }

//     SPtr<VulkanVertexLayout> GetCurrentVertexLayout()
//     {
//         return currentState.vertexLayout;
//     }

//     void SetCurrentVertexLayout(const SPtr<VulkanVertexLayout> &layout)
//     {
//         currentState.vertexLayout = layout;
//     }

//     StateData &GetCurrentStateData()
//     {
//         return currentState;
//     }

//     FrameData &GetCurrentFrameData()
//     {
//         return frames[currentFrameIndex];
//     }

//     SPtr<VulkanCommandPool> GetRenderCommandPool()
//     {
//         return renderCommandPool;
//     }

//     VulkanResourceRegistry<VulkanShader> &GetShaderRegistry()
//     {
//         return shaderRegistry;
//     }

//     virtual void Init(RenderWindow *window, const ContextDesc &desc) override;
//     virtual void Destroy() override;
//     virtual void ResizeSwapChain(uint32 width, uint32 height) override;

//     void Submit(const SPtr<VulkanCommandBuffer> &buffer);
//     void Present();

//     SPtr<VulkanRenderPipeline> GetRenderPipeline();

// private:
//     void CreateInstance();
//     void CreateDevice();
//     void CreateSurface();
//     void CreateSwapChain();
//     void CreatePresentFences();

//     void CreateDebugger();
//     void DestroyDebugger();
//     void CreateShaderCompiler();
//     void DestroyShaderCompiler();
//     void CreateVmaAllocator();
//     void DestroyVmaAllocator();
//     void CreateFrameDatas();
//     void DestroyFrameDatas();
//     void CreateCommandPools();
//     void DestroyCommandPools();

// private:
//     VkInstance instance = VK_NULL_HANDLE;
//     VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
//     VmaAllocator allocator = VK_NULL_HANDLE;
//     VkSurfaceKHR surface = VK_NULL_HANDLE;
//     VkSwapchainKHR swapChain = VK_NULL_HANDLE;
   
//     Array<VulkanFence> presentFences;
//     uint32 backBufferCount;

//     VulkanShaderCompiler *shaderCompiler = nullptr;

//     SPtr<VulkanDevice> device;
//     SPtr<VulkanCommandPool> renderCommandPool;

//     Array<RenderPipelineStorage> renderPipelines;
//     VulkanResourceRegistry<VulkanShader> shaderRegistry;

//     FrameData frames[VULKAN_FRAME_NUM];
//     uint32 currentFrameIndex = 0;

//     StateData currentState;
// };
// }
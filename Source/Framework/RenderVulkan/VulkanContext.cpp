// #include "Utils/DynamicLib.h"
// #include "RenderVulkan/VulkanContext.h"
// #include "RenderVulkan/VulkanQueue.h"
// #include "RenderVulkan/VulkanSync.h"
// #include "RenderVulkan/VulkanRenderWindow.h"
// #include "RenderVulkan/VulkanSwapChain.h"
// #include "RenderVulkan/VulkanRenderPipeline.h"
// #include "RenderVulkan/VulkanFrameBuffer.h"
// #include "RenderVulkan/VulkanCommandBuffer.h"
// #include "RenderVulkan/VulkanShader.h"
// #include "RenderVulkan/Private/VulkanShaderCompiler.h"

// namespace RenderCore
// {

// void VulkanContext::Init(RenderWindow *window, const ContextDesc &desc)
// {
//     this->window = window;
//     this->desc = desc;

//     CreateShaderCompiler();
//     CreateInstance();
//     CreateDebugger();
//     CreateDevice();
//     CreateSurface();
//     CreateSwapChain();
//     CreatePresentFences();
//     CreateVmaAllocator();

//     CreateFrameDatas();
//     CreateCommandPools();
// }

// void VulkanContext::Destroy()
// {
//     device->WaitIdle();

//     shaderRegistry.Cleanup();

//     DestroyCommandPools();
//     DestroyFrameDatas();

//     for (auto &e : renderPipelines)
//         e.renderPipeline->Destroy();
//     renderPipelines.Clear();

//     if (swapChain)
//         swapChain->Destroy();
//     if (surface != VK_NULL_HANDLE)
//         vkDestroySurfaceKHR(instance, surface, gVulkanAlloc);

//     DestroyVmaAllocator();
//     device->Destroy();

//     DestroyDebugger();

//     vkDestroyInstance(instance, gVulkanAlloc);

//     DestroyShaderCompiler();
// }

// void VulkanContext::Submit(const SPtr<VulkanCommandBuffer> &buffer)
// {
//     auto &frameData = frames[currentFrameIndex];

//     VkSubmitInfo submitInfo = {};
//     submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

//     VkSemaphore waitSemaphores[] = {frameData.swapChainImageSemaphore->GetHandle()};
//     VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
//     submitInfo.waitSemaphoreCount = 1;
//     submitInfo.pWaitSemaphores = waitSemaphores;
//     submitInfo.pWaitDstStageMask = waitStages;

//     auto commandBuffer = buffer->GetHandle();
//     submitInfo.commandBufferCount = 1;
//     submitInfo.pCommandBuffers = &commandBuffer;

//     VkSemaphore signalSemaphores[] = {frameData.renderFinishedSemaphore->GetHandle()};
//     submitInfo.signalSemaphoreCount = 1;
//     submitInfo.pSignalSemaphores = signalSemaphores;

//     if (vkQueueSubmit(buffer->GetQueue()->GetHandle(), 1, &submitInfo, frameData.fence->GetHandle()) != VK_SUCCESS)
//         CT_EXCEPTION(RenderCore, "Submit failed.");
// }

// void VulkanContext::Present()
// {
//     VkPresentInfoKHR presentInfo = {};
//     presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
//     presentInfo.pNext = nullptr;

//     VkSemaphore waitSemaphores[] = {frames[currentFrameIndex].renderFinishedSemaphore->GetHandle()};
//     presentInfo.waitSemaphoreCount = 1;
//     presentInfo.pWaitSemaphores = waitSemaphores;

//     uint32 imageIndex = swapChain->GetCurrentBackBufferIndex();
//     VkSwapchainKHR swapChains[] = {swapChain->GetHandle()};
//     presentInfo.swapchainCount = 1;
//     presentInfo.pSwapchains = swapChains;
//     presentInfo.pImageIndices = &imageIndex;

//     vkQueuePresentKHR(device->GetGraphicsQueue()->GetHandle(), &presentInfo);

//     currentFrameIndex = (currentFrameIndex + 1) % VULKAN_FRAME_NUM;
// }

// SPtr<VulkanRenderPipeline> VulkanContext::GetRenderPipeline()
// {
//     // if (!currentState.IsValid())
//     // {
//     //     CT_EXCEPTION(RenderCore, "Current state is valid.");
//     //     return nullptr;
//     // }

//     // // TODO Remove no longer used.
//     // auto renderPass = currentState.frameBuffer->GetRenderPass();
//     // for (const auto &e : renderPipelines)
//     // {
//     //     if (e.renderPipelineState.lock() == currentState.renderPipelineState &&
//     //         e.vertexLayout.lock() == currentState.vertexLayout &&
//     //         e.renderPass.lock() == renderPass)
//     //     {
//     //         return e.renderPipeline;
//     //     }
//     // }

//     // VulkanRenderPipelineCreateParams params;
//     // params.renderPass = renderPass;
//     // params.state = currentState.renderPipelineState;
//     // params.vertexLayout = currentState.vertexLayout;
//     // SPtr<VulkanRenderPipeline> result = VulkanRenderPipeline::Create(params);
//     // RenderPipelineStorage storage;
//     // storage.renderPipeline = result;
//     // storage.vertexLayout = params.vertexLayout;
//     // storage.renderPipelineState = params.state;
//     // storage.renderPass = params.renderPass;
//     // renderPipelines.Add(storage);
//     // return result;

//     return nullptr;
// }

// void VulkanContext::CreateFrameDatas()
// {
//     for (int32 i = 0; i < VULKAN_FRAME_NUM; ++i)
//     {
//         frames[i].renderFinishedSemaphore = VulkanSemaphore::Create();
//         frames[i].swapChainImageSemaphore = VulkanSemaphore::Create();
//         frames[i].fence = VulkanFence::Create();
//     }
// }

// void VulkanContext::DestroyFrameDatas()
// {
//     for (int32 i = 0; i < VULKAN_FRAME_NUM; ++i)
//     {
//         frames[i].renderFinishedSemaphore->Destroy();
//         frames[i].swapChainImageSemaphore->Destroy();
//         frames[i].fence->Destroy();
//     }
// }

// void VulkanContext::CreateCommandPools()
// {
//     VulkanCommandPoolCreateParams params;
//     params.familyIndex = device->GetGraphicsQueueFamilyIndex();
//     params.queue = device->GetGraphicsQueue();
//     renderCommandPool = VulkanCommandPool::Create(params);
// }

// void VulkanContext::DestroyCommandPools()
// {
//     renderCommandPool->Destroy();
// }

// void VulkanContext::CreateShaderCompiler()
// {
//     DynamicLib shaderCompilerLib(CT_TEXT("VulkanShaderCompiler"));
//     auto func = (GetVulkanShaderCompilerFunc)shaderCompilerLib.GetSymbol(CT_TEXT("GetVulkanShaderCompiler"));
//     shaderCompiler = func();
//     shaderCompiler->Init();
// }

// void VulkanContext::DestroyShaderCompiler()
// {
//     if(shaderCompiler)
//     {
//         shaderCompiler->Deinit();
//         Memory::Free(shaderCompiler);
//         shaderCompiler = nullptr;
//     }
// }

// }
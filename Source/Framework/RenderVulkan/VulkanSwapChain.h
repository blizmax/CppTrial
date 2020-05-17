#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

class VulkanSwapChain : public IVulkanResource
{
public:
    struct BackBufferData
    {
        SPtr<VulkanImage> image;
        SPtr<VulkanFrameBuffer> frameBuffer;
        SPtr<VulkanFence> fence;
    };

    VulkanSwapChain();
    ~VulkanSwapChain();

    virtual void Destroy() override;
    void Recreate(uint32 width, uint32 height);
    void AcquireBackBuffer();

    VkSwapchainKHR GetHandle() const
    {
        return swapChain;
    }

    uint32 GetWidth() const
    {
        return width;
    }

    uint32 GetHeight() const
    {
        return height;
    }

    uint32 GetCurrentBackBufferIndex() const
    {
        return currentBackBufferIndex;
    }

    uint32 GetBackBufferCount() const
    {
        return backBuffers.Count();
    }

    BackBufferData &GetCurentBackBufferData()
    {
        return backBuffers[currentBackBufferIndex];
    }

    static SPtr<VulkanSwapChain> Create(uint32 width, uint32 height);

private:
    VkSurfaceFormatKHR ChooseSurfaceFormat(const Array<VkSurfaceFormatKHR> &formats, bool gamma = false);
    VkPresentModeKHR ChoosePresentMode(const Array<VkPresentModeKHR> &modes, bool vsync = false);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, uint32 width, uint32 height);

private:
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    uint32 width = 0;
    uint32 height = 0;
    SPtr<VulkanRenderPass> renderPass;
    Array<BackBufferData> backBuffers;
    uint32 currentBackBufferIndex = 0;
};
}
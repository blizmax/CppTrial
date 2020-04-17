#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

struct VulkanSwapChainCreateParams
{
    VkSurfaceKHR surface;
    uint32 width;
    uint32 height;
};

class VulkanSwapChain : public IVulkanResource
{
public:
    struct SurfaceData
    {
        SPtr<VulkanImage> image;
        SPtr<VulkanFrameBuffer> frameBuffer;
    };

    VulkanSwapChain();
    ~VulkanSwapChain();

    virtual void Destroy() override;
    void Rebuild(const VulkanSwapChainCreateParams &params);

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

    static SPtr<VulkanSwapChain> Create();

private:
    VkSurfaceFormatKHR ChooseSurfaceFormat(const Array<VkSurfaceFormatKHR> formats, bool gamma = false);
    VkPresentModeKHR ChoosePresentMode(const Array<VkPresentModeKHR> modes, bool vsync = false);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32 width, uint32 height);

private:
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    uint32 width = 0;
    uint32 height = 0;
    SPtr<VulkanRenderPass> renderPass;
    Array<SurfaceData> surfaces;
    int32 surfaceIndex = 0;
};
}
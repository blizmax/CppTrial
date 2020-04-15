#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{
class VulkanSwapChain : public IVulkanResource
{
public:
    VulkanSwapChain();
    ~VulkanSwapChain();

    virtual void Destroy() override;
    void Rebuild(VkSurfaceKHR surface, uint32 width, uint32 height);

    static SPtr<VulkanSwapChain> Create();

private:
    VkSurfaceFormatKHR ChooseSurfaceFormat(const Array<VkSurfaceFormatKHR> formats, bool gamma = false);
    VkPresentModeKHR ChoosePresentMode(const Array<VkPresentModeKHR> modes, bool vsync = false);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32 width, uint32 height);

private:
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    uint32 width = 0;
    uint32 height = 0;
    Array<SPtr<VulkanImage>> images;
};
}
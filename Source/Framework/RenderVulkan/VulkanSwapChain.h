#pragma once

#include "RenderVulkan/.Package.h"

namespace RenderCore
{
class VulkanSwapChain
{
public:
    VulkanSwapChain();

    void Rebuild(VkSurfaceKHR surface);

    static SPtr<VulkanSwapChain> Create();

private:
    VkSwapchainKHR swapChain;  
};
}
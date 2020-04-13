#pragma once

#include "RenderVulkan/.Package.h"

namespace RenderCore
{
class VulkanSwapChain
{
public:
    static SPtr<VulkanSwapChain> Create();

private:
    VkSwapchainKHR swapChain;  
};
}
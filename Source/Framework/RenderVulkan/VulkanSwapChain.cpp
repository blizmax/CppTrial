#include "RenderVulkan/VulkanSwapChain.h"

namespace RenderCore
{
SPtr<VulkanSwapChain> VulkanSwapChain::Create()
{
    return Memory::MakeShared<VulkanSwapChain>();
}

VulkanSwapChain::VulkanSwapChain()
{
}

void VulkanSwapChain::Rebuild(VkSurfaceKHR surface)
{
}

}
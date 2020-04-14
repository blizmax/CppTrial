#include "RenderVulkan/VulkanSwapChain.h"
#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/VulkanImage.h"
#include "RenderVulkan/VulkanContext.h"
#include "Core/Math.h"

namespace RenderCore
{
SPtr<VulkanSwapChain> VulkanSwapChain::Create()
{
    return Memory::MakeShared<VulkanSwapChain>();
}

VulkanSwapChain::VulkanSwapChain()
{
}

VulkanSwapChain::~VulkanSwapChain()
{
}

void VulkanSwapChain::Destroy()
{
    for(auto &e : images)
        e->Destroy();
    images.Clear();

    if (swapChain != VK_NULL_HANDLE)
    {
        auto device = VulkanContext::Get().GetDevice();
        auto logicDevice = device->GetLogicalDeviceHandle();
        vkDestroySwapchainKHR(logicDevice, swapChain, gVulkanAlloc);

        swapChain = VK_NULL_HANDLE;
    }
}

void VulkanSwapChain::Rebuild(VkSurfaceKHR surface, uint32 width, uint32 height)
{
    Destroy();

    auto device = VulkanContext::Get().GetDevice();
    auto logicDevice = device->GetLogicalDeviceHandle();
    auto physicalDevice = device->GetPhysicalDeviceHandle();

    VkSurfaceCapabilitiesKHR surfaceCaps;
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCaps) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Get surface capabilities failed.");

    Array<VkSurfaceFormatKHR> formats;
    uint32 formatCount;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Get surface formats failed.");
    formats.AppendUninitialized(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.GetData());

    Array<VkPresentModeKHR> presentModes;
    uint32 presentModeCount;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Get surface present modes failed.");
    presentModes.AppendUninitialized(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.GetData());

    if (formatCount == 0 || presentModeCount == 0)
        CT_EXCEPTION(RenderCore, "No suitable format or present mode.");

    VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(formats);
    VkPresentModeKHR presentMode = ChoosePresentMode(presentModes);
    VkExtent2D extent = ChooseSwapExtent(surfaceCaps, width, height);
    uint32 imageCount = surfaceCaps.minImageCount;

    this->width = extent.width;
    this->height = extent.height;

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
    createInfo.preTransform = surfaceCaps.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    //TODO
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(logicDevice, &createInfo, gVulkanAlloc, &swapChain) != VK_SUCCESS) 
        CT_EXCEPTION(RenderCore, "Create swap chain failed.");

    Array<VkImage> swapChainImages;
    if(vkGetSwapchainImagesKHR(logicDevice, swapChain, &imageCount, nullptr) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Get swap chain images failed.");
    swapChainImages.AppendUninitialized(imageCount);
    vkGetSwapchainImagesKHR(logicDevice, swapChain, &imageCount, swapChainImages.GetData());
    
    for(int32 i = 0; i < imageCount; ++i)
    {
        VulkanImageCreateParams params;
        params.image = swapChainImages[i];
        params.format = surfaceFormat.format;
        params.layout = VK_IMAGE_LAYOUT_UNDEFINED;

        images.Add(VulkanImage::Create(params, false));
    }
}

VkSurfaceFormatKHR VulkanSwapChain::ChooseSurfaceFormat(const Array<VkSurfaceFormatKHR> formats, bool gamma)
{
    VkSurfaceFormatKHR desired = {VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    if (gamma)
        desired.format = VK_FORMAT_R8G8B8A8_SRGB;

    for (const auto &e : formats)
    {
        if (e.format == desired.format && e.colorSpace == desired.colorSpace)
            return e;
    }
    CT_LOG(Warning, CT_TEXT("Cannot find desired surface format, use first format instead."));
    return formats[0];
}

VkPresentModeKHR VulkanSwapChain::ChoosePresentMode(const Array<VkPresentModeKHR> modes, bool vsync)
{
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    if (!vsync)
    {
        for (int32 i = 0; i < modes.Count(); ++i)
        {
            if (modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
            {
                presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
                break;
            }
            if (modes[i] == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
                presentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
        }
    }
    else
    {
        for (int32 i = 0; i < modes.Count(); ++i)
        {
            if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                break;
            }
        }
    }
    return presentMode;
}

VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, uint32 width, uint32 height)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
        return capabilities.currentExtent;

    width = Math::Clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    height = Math::Clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    return {width, height};
}

}

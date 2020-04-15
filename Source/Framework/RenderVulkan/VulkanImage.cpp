#include "RenderVulkan/VulkanImage.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
SPtr<VulkanImage> VulkanImage::Create(const VulkanImageCreateParams &params, bool ownsImage)
{
    return Memory::MakeShared<VulkanImage>(params, ownsImage);
}

VulkanImage::VulkanImage(const VulkanImageCreateParams &params, bool ownsImage)
    : image(params.image), ownsImage(ownsImage)
{
    CreateImageView(params);
}

VulkanImage::~VulkanImage()
{
}

void VulkanImage::Destroy()
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();

    if(imageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(device, imageView, gVulkanAlloc);
        imageView = VK_NULL_HANDLE;
    }    
    if(ownsImage && image != VK_NULL_HANDLE)
    {
        vkDestroyImage(device, image, gVulkanAlloc);
        image = VK_NULL_HANDLE;
    }
}

void VulkanImage::CreateImageView(const VulkanImageCreateParams &params)
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();

    VkImageViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.image = params.image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;//TODO
    createInfo.format = params.format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device, &createInfo, gVulkanAlloc, &imageView) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create image view failed.");

    format = params.format;
}

}
#include "RenderVulkan/VulkanTexture.h"
#include "RenderVulkan/VulkanContext.h"
#include "Core/Math.h"

namespace RenderCore
{
VulkanTexture::VulkanTexture(uint32 width, uint32 height, uint32 depth, uint32 arrayLayers, uint32 mipLevels, uint32 sampleCount, ResourceFormat format, ResourceType resourceType, ResourceBindFlags flags, const void *data)
    : Texture(width, height, depth, arrayLayers, mipLevels, sampleCount, format, resourceType, flags, data)
{
    VkImageUsageFlags usageFlags = ToVkImageUsage(bindFlags);
    usageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.extent.height = height;
    imageInfo.extent.width = width;
    imageInfo.extent.depth = depth;
    imageInfo.format = ToVkResourceFormat(format);
    imageInfo.imageType = ToVkImageType(resourceType);
    imageInfo.initialLayout = data ? VK_IMAGE_LAYOUT_PREINITIALIZED : VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.arrayLayers = arrayLayers;
    imageInfo.mipLevels = mipLevels;
    imageInfo.pQueueFamilyIndices = nullptr;
    imageInfo.queueFamilyIndexCount = 0;
    imageInfo.samples = ToVkSampleCount(sampleCount);
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.usage = usageFlags;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL; //TODO Get image tiling by format and usage

    if (resourceType == ResourceType::TextureCube)
    {
        imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        imageInfo.arrayLayers *= 6;
    }

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    vmaCreateImage(gVulkanContext->GetVmaAllocator(), &imageInfo, &allocInfo, &textureData.image, &textureData.allocation, nullptr);

    //TODO Upload data
}

VulkanTexture::~VulkanTexture()
{
    if (textureData.allocation != VK_NULL_HANDLE)
    {
        auto allocator = gVulkanContext->GetVmaAllocator();
        vmaDestroyImage(allocator, textureData.image, textureData.allocation);

        textureData.allocation = VK_NULL_HANDLE;
        textureData.image = VK_NULL_HANDLE;
    }
}

}
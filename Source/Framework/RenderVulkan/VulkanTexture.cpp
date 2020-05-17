#include "RenderVulkan/VulkanTexture.h"
#include "RenderVulkan/VulkanDevice.h"
#include "Core/Math.h"

namespace RenderCore
{

SPtr<Texture> Texture::Create(uint32 width, uint32 height, uint32 depth, ResourceFormat format, ResourceType resourceType, uint32 arrayLayers, uint32 mipLevels, uint32 sampleCount, const void *data, ResourceBindFlags flags)
{
    auto ptr = Memory::MakeShared<VulkanTexture>(width, height, depth, arrayLayers, mipLevels, sampleCount, format, resourceType, flags);
    ptr->weakThis = ptr;
    ptr->InitData(data, mipLevels == UINT32_MAX);
    return ptr;
}

SPtr<Texture> Texture::Create2D(uint32 width, uint32 height, ResourceFormat format, void *handle, ResourceBindFlags flags)
{
    auto ptr = Memory::MakeShared<VulkanTexture>(width, height, 1, 1, 1, 1, format, ResourceType::Texture2D, flags);
    ptr->weakThis = ptr;
    ptr->ownsImage = false;
    ptr->textureData.image = (VkImage)handle;
    return ptr;
}

VulkanTexture::VulkanTexture(uint32 width, uint32 height, uint32 depth, uint32 arrayLayers, uint32 mipLevels, uint32 sampleCount, ResourceFormat format, ResourceType resourceType, ResourceBindFlags flags)
    : Texture(width, height, depth, arrayLayers, mipLevels, sampleCount, format, resourceType, flags)
{

}

VulkanTexture::~VulkanTexture()
{
    if (textureData.allocation != VK_NULL_HANDLE)
    {
        auto allocator = gVulkanDevice->GetVmaAllocator();
        vmaDestroyImage(allocator, textureData.image, textureData.allocation);
        textureData.allocation = VK_NULL_HANDLE;
        textureData.image = VK_NULL_HANDLE;
    }
}

void VulkanTexture::InitData(const void *data, bool genMips)
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
    vmaCreateImage(gVulkanDevice->GetVmaAllocator(), &imageInfo, &allocInfo, &textureData.image, &textureData.allocation, nullptr);


    //TODO
}

}
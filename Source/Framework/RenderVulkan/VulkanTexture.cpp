#include "RenderVulkan/VulkanTexture.h"
#include "RenderVulkan/VulkanDevice.h"
#include "Core/Math.h"

SPtr<Texture> Texture::Create(int32 width, int32 height, int32 depth, ResourceFormat format, ResourceType resourceType, int32 arrayLayers, int32 mipLevels, int32 sampleCount, const void *data, ResourceBindFlags flags)
{
    auto ptr = Memory::MakeShared<VulkanTexture>(width, height, depth, arrayLayers, mipLevels, sampleCount, format, resourceType, flags);
    ptr->weakThis = ptr;
    ptr->InitData(data, mipLevels == -1);
    return ptr;
}

SPtr<Texture> Texture::Create2D(int32 width, int32 height, ResourceFormat format, void *handle, ResourceBindFlags flags)
{
    auto ptr = Memory::MakeShared<VulkanTexture>(width, height, 1, 1, 1, 1, format, ResourceType::Texture2D, flags);
    ptr->weakThis = ptr;
    ptr->ownsImage = false;
    ptr->textureData.image = (VkImage)handle;
    return ptr;
}

VulkanTexture::VulkanTexture(int32 width, int32 height, int32 depth, int32 arrayLayers, int32 mipLevels, int32 sampleCount, ResourceFormat format, ResourceType resourceType, ResourceBindFlags flags)
    : Texture(width, height, depth, arrayLayers, mipLevels, sampleCount, format, resourceType, flags)
{

}

VulkanTexture::~VulkanTexture()
{
    if (textureData.allocation != VK_NULL_HANDLE)
    {
        if (gVulkanDevice)
        {
            gVulkanDevice->Release([image = textureData.image, allocation = textureData.allocation]() {
                auto allocator = gVulkanDevice->GetVmaAllocator();
                vmaDestroyImage(allocator, image, allocation);
            });
        }
        textureData.allocation = VK_NULL_HANDLE;
        textureData.image = VK_NULL_HANDLE;
    }
}

static VkFormatFeatureFlags GetFormatFeatureBitsFromUsage(VkImageUsageFlags usage)
{
    CT_CHECK((usage & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) == 0);
    CT_CHECK((usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) == 0);

    VkFormatFeatureFlags bits = 0;
    if (usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
        bits |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
    if (usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
        bits |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
    if (usage & VK_IMAGE_USAGE_SAMPLED_BIT)
        bits |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
    if (usage & VK_IMAGE_USAGE_STORAGE_BIT)
        bits |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
    if (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
        bits |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
    if (usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
        bits |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

    return bits;
}

static VkImageTiling GetFormatImageTiling(VkFormat format, VkImageUsageFlags usage)
{
    VkFormatProperties p;
    vkGetPhysicalDeviceFormatProperties(gVulkanDevice->GetPhysicalDeviceHandle(), format, &p);
    auto featureBits = GetFormatFeatureBitsFromUsage(usage);
    if ((p.optimalTilingFeatures & featureBits) == featureBits)
        return VK_IMAGE_TILING_OPTIMAL;
    if ((p.linearTilingFeatures & featureBits) == featureBits)
        return VK_IMAGE_TILING_LINEAR;
    
    CT_EXCEPTION(RenderCore, "Get imagee tiling failed");
    return VK_IMAGE_TILING_OPTIMAL;
}

void VulkanTexture::InitData(const void *data, bool autoGenMips)
{
    VkImageUsageFlags usageFlags = ToVkImageUsage(bindFlags);
    usageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.extent.width = CT_ALIGN(GetResourceFormatWidthCompressionRatio(format), width);
    imageInfo.extent.height = CT_ALIGN(GetResourceFormatHeightCompressionRatio(format), height);;
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
    imageInfo.tiling = GetFormatImageTiling(imageInfo.format, imageInfo.usage);

    if (resourceType == ResourceType::TextureCube)
    {
        imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        imageInfo.arrayLayers *= 6;
    }
    stateData.state = data ? ResourceState::PreInitialized : ResourceState::Undefined;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    if(vmaCreateImage(gVulkanDevice->GetVmaAllocator(), &imageInfo, &allocInfo, &textureData.image, &textureData.allocation, nullptr) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create image failed.");

    if(data)
    {
        auto context = gVulkanDevice->GetRenderContext();
        if(autoGenMips)
        {
            int32 sliceSize = width * height * GetResourceFormatBytes(format);
            const uint8 *src = reinterpret_cast<const uint8 *>(data);
            int32 count = arrayLayers;
            if(resourceType == ResourceType::TextureCube)
                count *= 6;
            
            for (int32 i = 0; i < count; i++)
            {
                int32 subresource = GetSubresourceIndex(i, 0);
                int32 mipLevel = 0; //GetSubresourceMipLevel(subresource);
                int32 w = GetWidth(mipLevel);
                int32 h = GetHeight(mipLevel);
                int32 d = GetDepth(mipLevel);
                context->UpdateSubresource(this, subresource, src, {0, 0, 0}, {w, h, d});
                src += sliceSize;
            }
        }
        else
        {
            context->UpdateTexture(this, data);
        }

        if(autoGenMips)
        {
            GenerateMips(context);
            ClearViews();
        }
    }
}
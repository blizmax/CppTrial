#include "RenderVulkan/VulkanResourceView.h"
#include "RenderVulkan/VulkanBuffer.h"
#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/VulkanTexture.h"

SPtr<ResourceView> ResourceView::CreateSrv(const SPtr<Buffer> &buffer, int32 first, int32 count)
{
    return Memory::MakeShared<VulkanBufferView>(buffer, first, count);
}

SPtr<ResourceView> ResourceView::CreateUav(const SPtr<Buffer> &buffer, int32 first, int32 count)
{
    return Memory::MakeShared<VulkanBufferView>(buffer, first, count);
}

SPtr<ResourceView> ResourceView::CreateCbv(const SPtr<Buffer> &buffer)
{
    return Memory::MakeShared<VulkanBufferView>(buffer, 0, 1);
}

SPtr<ResourceView> ResourceView::CreateSrv(const SPtr<Texture> &texture, int32 mostDetailedMip, int32 mipLevels, int32 firstArraySlice, int32 arrayLayers)
{
    return Memory::MakeShared<VulkanImageView>(texture, mostDetailedMip, mipLevels, firstArraySlice, arrayLayers);
}

SPtr<ResourceView> ResourceView::CreateUav(const SPtr<Texture> &texture, int32 mipLevel, int32 firstArraySlice, int32 arrayLayers)
{
    return Memory::MakeShared<VulkanImageView>(texture, mipLevel, 1, firstArraySlice, arrayLayers);
}

SPtr<ResourceView> ResourceView::CreateRtv(const SPtr<Texture> &texture, int32 mipLevel, int32 firstArraySlice, int32 arrayLayers)
{
    return Memory::MakeShared<VulkanImageView>(texture, mipLevel, 1, firstArraySlice, arrayLayers);
}

SPtr<ResourceView> ResourceView::CreateDsv(const SPtr<Texture> &texture, int32 mipLevel, int32 firstArraySlice, int32 arrayLayers)
{
    return Memory::MakeShared<VulkanImageView>(texture, mipLevel, 1, firstArraySlice, arrayLayers);
}

VulkanBufferView::VulkanBufferView(const WPtr<Resource> &resource, int32 firstElement, int32 elementCount)
    : ResourceView(resource, firstElement, elementCount)
{
    auto buffer = dynamic_cast<VulkanBuffer *>(GetResource());
    if (buffer && buffer->IsTyped())
    {
        VkBufferViewCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
        bufferInfo.buffer = buffer->GetHandle();
        bufferInfo.offset = 0;
        bufferInfo.range = VK_WHOLE_SIZE;
        bufferInfo.format = ToVkResourceFormat(buffer->GetResourceFormat());

        if (vkCreateBufferView(gVulkanDevice->GetLogicalDeviceHandle(), &bufferInfo, gVulkanAlloc, &bufferView) != VK_SUCCESS)
            CT_EXCEPTION(RenderCore, "Create buffer view failed.");
    }
}

VulkanBufferView::~VulkanBufferView()
{
    if (bufferView != VK_NULL_HANDLE)
    {
        if (gVulkanDevice)
        {
            gVulkanDevice->Release([bufferView = bufferView]() {
                vkDestroyBufferView(gVulkanDevice->GetLogicalDeviceHandle(), bufferView, gVulkanAlloc);
            });
        }
        bufferView = VK_NULL_HANDLE;
    }
}

VulkanImageView::VulkanImageView(const WPtr<Resource> &resource, int32 mostDetailedMip, int32 mipLevels, int32 firstArraySlice, int32 arrayLayers)
    : ResourceView(resource, mostDetailedMip, mipLevels, firstArraySlice, arrayLayers)
{
    auto texture = dynamic_cast<VulkanTexture *>(GetResource());
    if (texture)
    {
        if (texture->GetResourceType() == ResourceType::TextureCube)
        {
            firstArraySlice *= 6;
            arrayLayers *= 6;
        }

        VkImageViewCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageInfo.image = texture->GetHandle();
        imageInfo.viewType = ToVkImageViewType(texture->GetResourceType(), texture->GetArrayLayers() > 1);
        imageInfo.format = ToVkResourceFormat(texture->GetResourceFormat());
        imageInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageInfo.subresourceRange.aspectMask = ToVkImageAspect(texture->GetResourceFormat(), true);
        imageInfo.subresourceRange.baseMipLevel = mostDetailedMip;
        imageInfo.subresourceRange.levelCount = mipLevels;
        imageInfo.subresourceRange.baseArrayLayer = firstArraySlice;
        imageInfo.subresourceRange.layerCount = arrayLayers;

        if (vkCreateImageView(gVulkanDevice->GetLogicalDeviceHandle(), &imageInfo, gVulkanAlloc, &imageView) != VK_SUCCESS)
            CT_EXCEPTION(RenderCore, "Create image view failed.");
    }
}

VulkanImageView::~VulkanImageView()
{
    if (imageView != VK_NULL_HANDLE)
    {
        if (gVulkanDevice)
        {
            gVulkanDevice->Release([imageView = imageView]() {
                vkDestroyImageView(gVulkanDevice->GetLogicalDeviceHandle(), imageView, gVulkanAlloc);
            });
        }
        imageView = VK_NULL_HANDLE;
    }
}
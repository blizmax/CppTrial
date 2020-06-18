#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/ResourceView.h"

class VulkanBufferView : public ResourceView
{
public:
    VulkanBufferView(const WPtr<Resource> &resource, int32 firstElement, int32 elementCount);
    virtual ~VulkanBufferView();

    VkBufferView GetHandle() const
    {
        return bufferView;
    }

private:
    VkBufferView bufferView = VK_NULL_HANDLE;
};

class VulkanImageView : public ResourceView
{
public:
    VulkanImageView(const WPtr<Resource> &resource, int32 mostDetailedMip, int32 mipLevels, int32 firstArraySlice, int32 arrayLayers);
    virtual ~VulkanImageView();

    VkImageView GetHandle() const
    {
        return imageView;
    }

private:    
    VkImageView imageView = VK_NULL_HANDLE;
};
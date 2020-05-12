#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/ResourceView.h"

namespace RenderCore
{
class VulkanBufferView : public ResourceView
{
public:
    VulkanBufferView(const WPtr<Resource> &resource, uint32 firstElement, uint32 elementCount);
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
    VulkanImageView(const WPtr<Resource> &resource, uint32 mostDetailedMip, uint32 mipLevels, uint32 firstArraySlice, uint32 arrayLayers);
    virtual ~VulkanImageView();

    VkImageView GetHandle() const
    {
        return imageView;
    }

private:    
    VkImageView imageView = VK_NULL_HANDLE;
};

}
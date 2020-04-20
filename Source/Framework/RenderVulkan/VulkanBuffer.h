#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{
struct VulkanBufferCreateParams
{
    uint32 size;
    VkBufferUsageFlags usageFlags;
};

class VulkanBuffer : public IVulkanResource
{
public:
    VulkanBuffer(const VulkanBufferCreateParams &params);
    
    virtual void Destroy() override;

protected:
    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
};

}
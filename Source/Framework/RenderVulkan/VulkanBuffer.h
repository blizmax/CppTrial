#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{
class VulkanBuffer : public IVulkanResource
{
public:
    virtual void Destroy() override;

    void Init(uint32 size, VkBufferUsageFlags usage);
    void *Map();
    void Unmap();

    uint32 GetSize() const
    {
        return size;
    }

    VkBuffer GetHandle() const
    {
        return buffer;
    }

protected:
    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
    uint32 size = 0;
};

}
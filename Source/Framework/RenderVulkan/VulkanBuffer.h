#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

enum class VulkanBufferType
{
    Vertex,
    Index,
    Uniform,
};

struct VulkanBufferCreateParams
{
    uint32 size;
    VulkanBufferType bufferType;
    GpuBufferUsage usage;
};

class VulkanBuffer : public IVulkanResource
{
public:
    struct BufferData
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
    };

    virtual void Destroy() override;

    void Init(const VulkanBufferCreateParams &params);
    void *Map();
    void Unmap();

    uint32 GetSize() const
    {
        return size;
    }

    GpuBufferUsage GetUsage() const
    {
        return usage;
    }

    VkBuffer GetHandle() const
    {
        return bufferData.buffer;
    }

private:
    BufferData CreateBuffer(bool staging);
    void DestroyBuffer(BufferData &data);

private:
    BufferData bufferData{};
    BufferData stagingBufferData{};
    bool useStaging = false;

    uint32 size = 0;
    VulkanBufferType bufferType;
    GpuBufferUsage usage;
};

}
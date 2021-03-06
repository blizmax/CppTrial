#pragma once

#include "RenderCore/Buffer.h"
#include "RenderVulkan/VulkanMemory.h"

class VulkanBuffer : public Buffer
{
public:
    struct BufferData
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
    };

    VulkanBuffer(uint32 size, ResourceBindFlags bindFlags, BufferCpuAccess access);
    virtual ~VulkanBuffer();

    virtual void *Map(BufferMapType mapType) override;
    virtual void Unmap() override;
    virtual void SetBlob(const void *data, uint32 offset, uint32 dataSize) override;

    VkBuffer GetHandle() const
    {
        return bufferData.buffer;
    }

private:
    BufferData CreateBuffer(ResourceBindFlags bindFlags, MemoryUsage memoryUsage);
    void DestroyBuffer(BufferData &data);

private:
    BufferData bufferData{};
    SPtr<Buffer> stagingBuffer;
};
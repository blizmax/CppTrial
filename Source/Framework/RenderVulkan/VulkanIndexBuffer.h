#pragma once

#include "RenderVulkan/VulkanBuffer.h"
#include "RenderCore/IndexBuffer.h"

namespace RenderCore
{

class VulkanIndexBuffer : public IndexBuffer, public IVulkanResource
{
public:
    VulkanIndexBuffer(const IndexBufferCreateParams &params);
    ~VulkanIndexBuffer();

    virtual void *Map() override;
    virtual void Unmap() override;
    virtual void Destroy() override;

    virtual uint32 GetSize() const override
    {
        return buffer.GetSize();
    }

    virtual GpuBufferUsage GetUsage() const override
    {
        return buffer.GetUsage();
    }

    VkBuffer GetHandle() const
    {
        return buffer.GetHandle();
    }

private:
    VulkanBuffer buffer;
};

}
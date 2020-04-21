#pragma once

#include "RenderVulkan/VulkanBuffer.h"
#include "RenderCore/VertexBuffer.h"

namespace RenderCore
{

class VulkanVertexBuffer : public VertexBuffer, public IVulkanResource
{
public:
    VulkanVertexBuffer(const VertexBufferCreateParams &params);
    ~VulkanVertexBuffer();

    virtual void *Map() override;
    virtual void Unmap() override;
    virtual void Destroy() override;

    virtual uint32 GetSize() const override
    {
        return buffer.GetSize();
    }

    VkBuffer GetHandle() const
    {
        return buffer.GetHandle();
    }

private:
    VulkanBuffer buffer;
};

}
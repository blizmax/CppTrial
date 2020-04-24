#pragma once

#include "RenderVulkan/VulkanBuffer.h"
#include "RenderCore/UniformBuffer.h"

namespace RenderCore
{

class VulkanUniformBuffer : public UniformBuffer, public IVulkanResource
{
public:
    VulkanUniformBuffer(const UniformBufferCreateParams &params);
    ~VulkanUniformBuffer();

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
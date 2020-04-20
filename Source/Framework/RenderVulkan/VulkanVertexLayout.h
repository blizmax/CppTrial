#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/VertexLayout.h"

namespace RenderCore
{
class VulkanVertexLayout : public VertexLayout
{
public:
    VulkanVertexLayout(const VertexLayoutCreateParams &params);

    virtual uint32 GetStride() const override
    {
        return stride;
    }

    virtual const Array<VertexAttribute> &GetAttributes() const override
    {
        return attributes;
    }

private:
    VkPipelineVertexInputStateCreateInfo createInfo;
    Array<VertexAttribute> attributes;
    uint32 stride;
};

}
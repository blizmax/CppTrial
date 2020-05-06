#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/VertexLayout.h"

namespace RenderCore
{
class VulkanVertexLayout : public VertexLayout
{
public:
    VulkanVertexLayout();

    virtual void AddBufferLayout(const SPtr<VertexBufferLayout> &layout) override
    {
        bufferLayouts.Add(layout);
    }

    virtual const Array<SPtr<VertexBufferLayout>> &GetBufferLayouts() const override
    {
        return bufferLayouts;
    }

private:
    friend class VulkanRenderPipeline;

    Array<SPtr<VertexBufferLayout>> bufferLayouts;

    Array<VkVertexInputBindingDescription> bindingDescs;
    Array<VkVertexInputAttributeDescription> attributeDescs;
    VkPipelineVertexInputStateCreateInfo createInfo;
};

}
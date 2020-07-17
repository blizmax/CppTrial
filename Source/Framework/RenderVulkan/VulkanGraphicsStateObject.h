#pragma once

#include "RenderCore/GraphicsStateObject.h"
#include "RenderVulkan/VulkanMemory.h"

class VulkanGraphicsStateObject : public GraphicsStateObject
{
public:
    VulkanGraphicsStateObject(const GraphicsStateObjectDesc &desc);

    virtual ~VulkanGraphicsStateObject();

    VkPipeline GetHandle() const
    {
        return pipeline;
    }

private:
    VkPipeline pipeline = VK_NULL_HANDLE;
};
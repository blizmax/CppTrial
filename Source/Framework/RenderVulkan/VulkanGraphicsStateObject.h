#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/GraphicsStateObject.h"

namespace RenderCore
{

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


}
#pragma once

#include "RenderCore/ComputeStateObject.h"
#include "RenderVulkan/VulkanMemory.h"

class VulkanComputeStateObject : public ComputeStateObject
{
public:
    VulkanComputeStateObject(const ComputeStateObjectDesc &desc);

    virtual ~VulkanComputeStateObject();

    VkPipeline GetHandle() const
    {
        return pipeline;
    }

private:
    VkPipeline pipeline = VK_NULL_HANDLE;
};
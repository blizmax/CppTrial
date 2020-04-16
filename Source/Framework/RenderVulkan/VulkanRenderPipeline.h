#pragma once

#include "RenderCore/RenderPipeline.h"
#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{
class VulkanRenderPipeline : public RenderPipeline, public IVulkanResource
{
public:
    VulkanRenderPipeline(const RenderPipelineCreateParams &params);

    virtual void Destroy() override;

    VkPipeline GetHandle() const
    {
        return pipeline;
    }

private:

private:
    VkPipeline pipeline = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
};
}
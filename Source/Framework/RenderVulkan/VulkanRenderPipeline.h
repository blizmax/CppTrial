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

private:

private:
    VkPipelineLayout pipelineLayout;
};
}
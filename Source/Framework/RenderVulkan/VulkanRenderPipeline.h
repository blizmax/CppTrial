#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{
class VulkanRenderPipeline : public VulkanResource
{
public:
    VulkanRenderPipeline();

    virtual void Destroy() override;

private:

private:
    VkPipelineLayout pipelineLayout;
};
}
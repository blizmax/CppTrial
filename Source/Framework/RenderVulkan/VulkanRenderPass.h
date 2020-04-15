#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

struct VulkanRenderPassCreateParams
{

};

class VulkanRenderPass : public IVulkanResource
{
public:
    VulkanRenderPass(const VulkanRenderPassCreateParams &params);

    virtual void Destroy() override;

    static SPtr<VulkanRenderPass> Create();

private:
    VkRenderPass renderPass;
};
}
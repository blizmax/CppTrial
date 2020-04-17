#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{
struct VulkanColorAttachmentDesc
{
    VkFormat format = VK_FORMAT_UNDEFINED;
    bool offscreen = false;
};

struct VulkanRenderPassCreateParams
{
    VulkanColorAttachmentDesc colorAttachmentDescs[COLOR_ATTCHMENT_MAX_NUM];
    int32 colorAttachmentCount = 0;
    VkSampleCountFlagBits sampleFlagBits = VK_SAMPLE_COUNT_1_BIT; 
};

class VulkanRenderPass : public IVulkanResource
{
public:
    VulkanRenderPass(const VulkanRenderPassCreateParams &params);
    virtual void Destroy() override;

    VkRenderPass GetHandle() const
    {
        return renderPass;
    }

    static SPtr<VulkanRenderPass> Create(const VulkanRenderPassCreateParams &params);

private:
    VkRenderPass renderPass = VK_NULL_HANDLE;
};
}
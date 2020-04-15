#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

struct VulkanFrameBufferCreateParams
{
    uint32 width = 0;
    uint32 height = 0;
    uint32 layers = 0;
    int32 colorAttachmentCount = 0;
    SPtr<VulkanImage> colorAttachments[CT_COLOR_ATTCHMENT_MAX_NUM];
    SPtr<VulkanRenderPass> renderPass; //Shared render pass
};

class VulkanFrameBuffer : public IVulkanResource
{
public:
    VulkanFrameBuffer(const VulkanFrameBufferCreateParams &params);

    virtual void Destroy() override;

    static SPtr<VulkanFrameBuffer> Create(const VulkanFrameBufferCreateParams &params);

private:
    VkFramebuffer frameBuffer = VK_NULL_HANDLE;
};
}
#pragma once

#include "RenderCore/FrameBuffer.h"
#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

struct VulkanFrameBufferCreateParams
{
    uint32 width = 0;
    uint32 height = 0;
    uint32 layers = 0;
    int32 colorAttachmentCount = 0;
    SPtr<VulkanImage> colorAttachments[COLOR_ATTCHMENT_MAX_NUM];
    SPtr<VulkanRenderPass> renderPass; //Shared render pass
};

class VulkanFrameBuffer : public FrameBuffer, public IVulkanResource
{
public:
    virtual void Destroy() override;
    virtual void Apply() override;

    SPtr<VulkanRenderPass> GetRenderPass() const
    {
        return renderPass;
    }

    VkFramebuffer GetHandle() const
    {
        return frameBuffer;
    }

private:
    VkFramebuffer frameBuffer = VK_NULL_HANDLE;
    SPtr<VulkanRenderPass> renderPass;
};
}
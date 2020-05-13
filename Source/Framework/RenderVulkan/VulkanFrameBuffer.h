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

class VulkanFrameBuffer : public FrameBuffer
{
public:
    virtual ~VulkanFrameBuffer();

    virtual void Apply() override;

    VkRenderPass GetRenderPassHandle() const
    {
        return renderPass;
    }

    VkFramebuffer GetHandle() const
    {
        return frameBuffer;
    }

private:
    void CreateRenderPass();

private:
    VkFramebuffer frameBuffer = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
};
}
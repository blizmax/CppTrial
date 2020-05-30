#pragma once

#include "RenderCore/FrameBuffer.h"
#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

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
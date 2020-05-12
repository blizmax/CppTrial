#include "RenderVulkan/VulkanFrameBuffer.h"
#include "RenderVulkan/VulkanRenderPass.h"
#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/VulkanImage.h"

namespace RenderCore
{
SPtr<FrameBuffer> FrameBuffer::Create(const Array<SPtr<Texture>> &colors, const SPtr<Texture> &depthStencil)
{
    auto ptr = Memory::MakeShared<VulkanFrameBuffer>();

    for(const auto &e : colors)
    {
        ptr->AddColorAttachment(e);
    }
    if(depthStencil)
    {
        ptr->SetDepthStencilAttachment(depthStencil);
    }
    ptr->Apply();

    return ptr;
}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
    if (frameBuffer != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer(gVulkanContext->GetLogicalDeviceHandle(), frameBuffer, gVulkanAlloc);
        frameBuffer = VK_NULL_HANDLE;
    }
}

void VulkanFrameBuffer::Apply()
{
    FrameBuffer::Apply();

    auto device = VulkanContext::Get().GetLogicalDeviceHandle();

    VkImageView imageViews[COLOR_ATTCHMENT_MAX_NUM + 1];
    for(int32 i = 0; i < colorAttachments.Count(); ++i)
    {
        auto rtv = GetRtv(i);
        const auto &viewInfo = rtv->GetViewInfo();
        //TODO
    }

    //TODO
}

}
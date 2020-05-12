#include "RenderVulkan/VulkanFrameBuffer.h"
#include "RenderVulkan/VulkanRenderPass.h"
#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/VulkanImage.h"
#include "RenderVulkan/VulkanResourceView.h"

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

    Array<VkImageView> imageViews;
    uint32 arrayLayers = UINT32_MAX;
    for(int32 i = 0; i < colorAttachments.Count(); ++i)
    {
        auto rtv = GetRtv(i);
        const auto &viewInfo = rtv->GetViewInfo();
        CT_CHECK(arrayLayers == UINT32_MAX || arrayLayers == viewInfo.arrayLayers);
        arrayLayers = viewInfo.arrayLayers;
        imageViews.Add(std::static_pointer_cast<VulkanImageView>(rtv)->GetHandle());
    }

    if(hasDepthStencil)
    {
        auto dsv = GetDsv();
        const auto &viewInfo = dsv->GetViewInfo();
        CT_CHECK(arrayLayers == UINT32_MAX || arrayLayers == viewInfo.arrayLayers);
        arrayLayers = viewInfo.arrayLayers;
        imageViews.Add(std::static_pointer_cast<VulkanImageView>(dsv)->GetHandle());
    }

    //TODO Create renderpass

    VkFramebufferCreateInfo frameBufferInfo = {};
    frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    //frameBufferInfo.renderPass = pass;
    frameBufferInfo.attachmentCount = imageViews.Count();
    frameBufferInfo.pAttachments = imageViews.GetData();
    frameBufferInfo.width = width;
    frameBufferInfo.height = height;
    frameBufferInfo.layers = arrayLayers;

    if (vkCreateFramebuffer(gVulkanContext->GetLogicalDeviceHandle(), &frameBufferInfo, gVulkanAlloc, &frameBuffer) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create framebuffer failed.");
}

}
#include "RenderVulkan/VulkanFrameBuffer.h"
#include "RenderVulkan/VulkanRenderPass.h"
#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/VulkanImage.h"

namespace RenderCore
{
SPtr<VulkanFrameBuffer> VulkanFrameBuffer::Create(const VulkanFrameBufferCreateParams &params)
{
    return Memory::MakeShared<VulkanFrameBuffer>(params);
}

VulkanFrameBuffer::VulkanFrameBuffer(const VulkanFrameBufferCreateParams &params)
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();
    VkImageView colorAttachs[COLOR_ATTCHMENT_MAX_NUM];
    for(int32 i = 0; i < params.colorAttachmentCount; ++i)
        colorAttachs[i] = params.colorAttachments[i]->GetViewHandle();

    if(params.renderPass != nullptr)
    {
        renderPass = params.renderPass;
    }
    else
    {
        //TODO
        //Create render pass
    }

    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass->GetHandle();
    framebufferInfo.attachmentCount = params.colorAttachmentCount;
    framebufferInfo.pAttachments = colorAttachs;
    framebufferInfo.width = params.width;
    framebufferInfo.height = params.height;
    framebufferInfo.layers = params.layers;

    if (vkCreateFramebuffer(device, &framebufferInfo, gVulkanAlloc, &frameBuffer) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create frame buffer failed.");

    width = params.width;
    height = params.height;
}

void VulkanFrameBuffer::Destroy()
{
    if (frameBuffer != VK_NULL_HANDLE)
    {
        auto device = VulkanContext::Get().GetLogicalDeviceHandle();
        vkDestroyFramebuffer(device, frameBuffer, gVulkanAlloc);
        frameBuffer = VK_NULL_HANDLE;
    }
}

}
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
    if(renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(gVulkanContext->GetLogicalDeviceHandle(), renderPass, gVulkanAlloc);
        renderPass = VK_NULL_HANDLE;
    }
    if (frameBuffer != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer(gVulkanContext->GetLogicalDeviceHandle(), frameBuffer, gVulkanAlloc);
        frameBuffer = VK_NULL_HANDLE;
    }
}

void VulkanFrameBuffer::Apply()
{
    FrameBuffer::Apply();

    CreateRenderPass();

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

    if(desc.hasDepthStencil)
    {
        auto dsv = GetDsv();
        const auto &viewInfo = dsv->GetViewInfo();
        CT_CHECK(arrayLayers == UINT32_MAX || arrayLayers == viewInfo.arrayLayers);
        arrayLayers = viewInfo.arrayLayers;
        imageViews.Add(std::static_pointer_cast<VulkanImageView>(dsv)->GetHandle());
    }

    VkFramebufferCreateInfo frameBufferInfo = {};
    frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferInfo.renderPass = renderPass;
    frameBufferInfo.attachmentCount = imageViews.Count();
    frameBufferInfo.pAttachments = imageViews.GetData();
    frameBufferInfo.width = width;
    frameBufferInfo.height = height;
    frameBufferInfo.layers = arrayLayers;

    if (vkCreateFramebuffer(gVulkanContext->GetLogicalDeviceHandle(), &frameBufferInfo, gVulkanAlloc, &frameBuffer) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create framebuffer failed.");
}

void VulkanFrameBuffer::CreateRenderPass()
{
    bool hasDepth = desc.hasDepthStencil;
    Array<VkAttachmentDescription> attachmentDescs;
    Array<VkAttachmentReference> colorAttachmentRefs;
    VkAttachmentReference depthAttachmentRef;
    Array<VkSubpassDescription> subpassDescs;
    Array<VkSubpassDependency> subpassDeps;

    for(int32 i = 0; i < desc.colors.Count(); ++i)
    {
        auto &attach = desc.colors[i];
        VkAttachmentDescription attachDesc = {};
        attachDesc.format = ToVkResourceFormat(attach.format);
        attachDesc.samples = ToVkSampleCount(desc.sampleCount);
        attachDesc.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachDesc.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        //attachDesc.finalLayout = attach.offscreen ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        attachmentDescs.Add(attachDesc);

        VkAttachmentReference attachRef = {};
        attachRef.attachment = i;
        attachRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachmentRefs.Add(attachRef);
    }

    if(hasDepth)
    {
        auto &attach = desc.depthStencil;
        VkAttachmentDescription attachDesc = {};
        attachDesc.format = ToVkResourceFormat(attach.format);
        attachDesc.samples = ToVkSampleCount(desc.sampleCount);
        attachDesc.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachDesc.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachDesc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachmentDescs.Add(attachDesc);

        depthAttachmentRef = {};
        depthAttachmentRef.attachment = attachmentDescs.Count() - 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }

    {
        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.inputAttachmentCount = 0;
        subpass.pInputAttachments = nullptr;
        subpass.preserveAttachmentCount = 0;
        subpass.pPreserveAttachments = nullptr;
        subpass.pResolveAttachments = nullptr;
        subpass.colorAttachmentCount = colorAttachmentRefs.Count();
        subpass.pColorAttachments = colorAttachmentRefs.GetData();
        subpass.pDepthStencilAttachment = hasDepth ? &depthAttachmentRef : nullptr;

        subpassDescs.Add(subpass);
    }

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = nullptr;
    renderPassInfo.flags = 0;
    renderPassInfo.attachmentCount = attachmentDescs.Count();
    renderPassInfo.pAttachments = attachmentDescs.GetData();
    renderPassInfo.subpassCount = subpassDescs.Count();
    renderPassInfo.pSubpasses = subpassDescs.GetData();
    renderPassInfo.dependencyCount = subpassDeps.Count();
    renderPassInfo.pDependencies = subpassDeps.GetData();

    if (vkCreateRenderPass(gVulkanContext->GetLogicalDeviceHandle(), &renderPassInfo, gVulkanAlloc, &renderPass) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create render pass failed.");
}

}
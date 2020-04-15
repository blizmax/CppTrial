#include "RenderVulkan/VulkanRenderPass.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
SPtr<VulkanRenderPass> VulkanRenderPass::Create(const VulkanRenderPassCreateParams &params)
{
    return Memory::MakeShared<VulkanRenderPass>(params);
}

VulkanRenderPass::VulkanRenderPass(const VulkanRenderPassCreateParams &params)
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();
    VkAttachmentDescription colorAttachments[CT_COLOR_ATTCHMENT_MAX_NUM];
    VkAttachmentReference colorAttachmentRefs[CT_COLOR_ATTCHMENT_MAX_NUM];
    int32 attachmentCount = params.colorAttachmentCount; //TODO depthStencil

    for (int32 i = 0; i < params.colorAttachmentCount; ++i)
    {
        auto &attach = params.colorAttachmentDescs[i];
        VkAttachmentDescription &colorAttachment = colorAttachments[i];
        colorAttachment.flags = 0;
        colorAttachment.format = attach.format;
        colorAttachment.samples = params.sampleFlagBits;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = attach.offscreen ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference &colorAttachmentRef = colorAttachmentRefs[i];
        colorAttachmentRef.attachment = i;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    VkSubpassDescription subpass = {};
    subpass.flags = 0;
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;
    subpass.pResolveAttachments = nullptr;
    subpass.colorAttachmentCount = params.colorAttachmentCount;
    subpass.pColorAttachments = colorAttachmentRefs;
    subpass.pDepthStencilAttachment = nullptr; //TODO

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = nullptr;
    renderPassInfo.flags = 0;
    renderPassInfo.attachmentCount = attachmentCount;
    renderPassInfo.pAttachments = colorAttachments;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 0;
    renderPassInfo.pDependencies = nullptr; //TODO

    if (vkCreateRenderPass(device, &renderPassInfo, gVulkanAlloc, &renderPass) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create render pass failed.");
}

void VulkanRenderPass::Destroy()
{
    if (renderPass != VK_NULL_HANDLE)
    {
        auto device = VulkanContext::Get().GetLogicalDeviceHandle();
        vkDestroyRenderPass(device, renderPass, gVulkanAlloc);
        renderPass = VK_NULL_HANDLE;
    }
}
}
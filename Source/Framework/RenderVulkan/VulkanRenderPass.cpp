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
    VkAttachmentDescription colorAttachments[COLOR_ATTCHMENT_MAX_NUM];
    VkAttachmentReference colorAttachmentRefs[COLOR_ATTCHMENT_MAX_NUM];
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

    VkSubpassDependency dependencies[2];
    uint32 dependencyCount = 1;
    // dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    // dependencies[0].dstSubpass = 0;
    // dependencies[0].srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    // dependencies[0].dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    // dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_SHADER_READ_BIT;
    // dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | 
    //     VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
    // dependencies[0].dependencyFlags = 0;
    // dependencies[1].srcSubpass = 0;
    // dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    // dependencies[1].srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    // dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    // dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | 
    //     VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
    // dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_SHADER_READ_BIT;
    // dependencies[1].dependencyFlags = 0;
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = 0;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = 0;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = nullptr;
    renderPassInfo.flags = 0;
    renderPassInfo.attachmentCount = attachmentCount;
    renderPassInfo.pAttachments = colorAttachments;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = dependencyCount;
    renderPassInfo.pDependencies = dependencies;

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
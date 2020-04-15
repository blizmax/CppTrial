#include "RenderVulkan/VulkanRenderPass.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
VulkanRenderPass::VulkanRenderPass(const VulkanRenderPassCreateParams &params)
{
    auto &device = VulkanContext::Get().GetDevice();
    auto logicalDevice = device->GetLogicalDeviceHandle();

    // VkAttachmentDescription colorAttachment = {};
    // colorAttachment.format = swapChainImageFormat;
    // colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    // VkRenderPassCreateInfo renderPassInfo = {};
    // renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    // renderPassInfo.pNext = nullptr;
    // renderPassInfo.flags = 0;
    // renderPassInfo.attachmentCount = 1;
    // renderPassInfo.pAttachments = &colorAttachment;
    // renderPassInfo.subpassCount = 1;
    // renderPassInfo.pSubpasses = &subpass;

    // if (vkCreateRenderPass(logicalDevice, &renderPassInfo, gVulkanAlloc, &renderPass) != VK_SUCCESS) {
    //     CT_EXCEPTION(RenderCore, "Create render pass failed.");
}
}
#include "RenderVulkan/VulkanCommandBuffer.h"
#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/VulkanFrameBuffer.h"
#include "RenderVulkan/VulkanRenderPass.h"
#include "RenderVulkan/VulkanRenderPipeline.h"

namespace RenderCore
{
VulkanCommandPool::VulkanCommandPool(uint32 familyIndex) : familyIndex(familyIndex)
{
    auto device = VulkanContext::Get().GetDevice();
    auto logicDevice = device->GetLogicalDeviceHandle();

    VkCommandPoolCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    createInfo.queueFamilyIndex = familyIndex;

    if(vkCreateCommandPool(logicDevice, &createInfo, gVulkanAlloc, &pool) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create command pool failed.");
}

void VulkanCommandPool::Destroy()
{
    buffers.Clear();

    auto logicDevice = VulkanContext::Get().GetLogicalDeviceHandle();
    if(pool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(logicDevice, pool, gVulkanAlloc);
        pool = VK_NULL_HANDLE;
    }
}

SPtr<VulkanCommandBuffer> VulkanCommandPool::CreateBuffer()
{
    //TODO
    return nullptr;
}

VulkanCommandBuffer::VulkanCommandBuffer(const VulkanCommandBufferCreateParams& params)
{
    auto logicDevice = VulkanContext::Get().GetLogicalDeviceHandle();

    VkCommandBufferAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.commandPool = params.pool;
    allocInfo.level = params.secondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if(vkAllocateCommandBuffers(logicDevice, &allocInfo, &buffer) != VK_SUCCESS)
		CT_EXCEPTION(RenderCore, "Create command buffer failed.");
}

void VulkanCommandBuffer::Begin()
{
    VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	beginInfo.pInheritanceInfo = nullptr;

	if(vkBeginCommandBuffer(buffer, &beginInfo) != VK_SUCCESS)
	    CT_EXCEPTION(RenderCore, "Begin command buffer failed.");
}

void VulkanCommandBuffer::End()
{
    if(vkEndCommandBuffer(buffer) != VK_SUCCESS)
	    CT_EXCEPTION(RenderCore, "End command buffer failed.");
}

void VulkanCommandBuffer::BeginRenderPass()
{
    auto frameBuffer = VulkanContext::Get().GetFrameBuffer();

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.pNext = nullptr;
    renderPassInfo.renderPass = frameBuffer->GetRenderPass()->GetHandle();
    renderPassInfo.framebuffer = frameBuffer->GetHandle();

    //TODO
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent.width = frameBuffer->GetWidth();
    renderPassInfo.renderArea.extent.height = frameBuffer->GetHeight();

    //TODO
    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandBuffer::EndRenderPass()
{
    vkCmdEndRenderPass(buffer);
}

void VulkanCommandBuffer::BindRenderPipeline()
{
    auto pipeline = VulkanContext::Get().GetRenderPipeline();
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetHandle());
}

void VulkanCommandBuffer::Draw(uint32 vertexOffset, uint32 vertexCount, uint32 instanceCount)
{
    vkCmdDraw(buffer, vertexCount, instanceCount, vertexOffset, 0);
}

}
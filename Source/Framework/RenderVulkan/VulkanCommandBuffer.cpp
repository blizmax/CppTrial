#include "RenderVulkan/VulkanCommandBuffer.h"
#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/VulkanFrameBuffer.h"
#include "RenderVulkan/VulkanRenderPass.h"
#include "RenderVulkan/VulkanRenderPipeline.h"

namespace RenderCore
{
SPtr<VulkanCommandPool> VulkanCommandPool::Create(const VulkanCommandPoolCreateParams &params)
{
    return Memory::MakeShared<VulkanCommandPool>(params);
}

VulkanCommandPool::VulkanCommandPool(const VulkanCommandPoolCreateParams &params) 
    : familyIndex(params.familyIndex), queue(params.queue)
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

SPtr<VulkanCommandBuffer> VulkanCommandPool::GetIdleBuffer()
{
    for(auto &e : buffers)
    {
        if(e->IsReady())
            return e;
    }

    buffers.Add(CreateBuffer());
    return buffers.Last();
}

SPtr<VulkanCommandBuffer> VulkanCommandPool::CreateBuffer()
{
    VulkanCommandBufferCreateParams params;
    params.pool = pool;
    params.secondary = false;
    params.queue = queue;
    return Memory::MakeShared<VulkanCommandBuffer>(params);
}

VulkanCommandBuffer::VulkanCommandBuffer(const VulkanCommandBufferCreateParams& params)
     : queue(params.queue)
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
    CT_CHECK(state == State::Ready);

    VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	beginInfo.pInheritanceInfo = nullptr;

	if(vkBeginCommandBuffer(buffer, &beginInfo) != VK_SUCCESS)
	    CT_EXCEPTION(RenderCore, "Begin command buffer failed.");

    state = State::Recording;
}

void VulkanCommandBuffer::End()
{
    CT_CHECK(state == State::Recording);

    if(vkEndCommandBuffer(buffer) != VK_SUCCESS)
	    CT_EXCEPTION(RenderCore, "End command buffer failed.");

    state == State::Recorded;
}

void VulkanCommandBuffer::BeginRenderPass()
{
    CT_CHECK(state == State::Recording);

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

    state = State::RecordingRenderPass;
}

void VulkanCommandBuffer::EndRenderPass()
{
    CT_CHECK(state == State::RecordingRenderPass);

    vkCmdEndRenderPass(buffer);

    state = State::Recording;
}

void VulkanCommandBuffer::Reset()
{
    //TODO
    // VkCommandBufferResetFlags flags = 0; //VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT
    // vkResetCommandBuffer(buffer, flags);

    // state = State::Ready;
}

void VulkanCommandBuffer::BindRenderPipeline()
{
    auto pipeline = VulkanContext::Get().GetRenderPipeline();
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetHandle());
}

void VulkanCommandBuffer::SetViewport(float x, float y, float width, float height)
{
    VkViewport viewport;
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(buffer, 0, 1, &viewport);
}

void VulkanCommandBuffer::SetScissor(int32 x, int32 y, uint32 width, uint32 height)
{
    VkRect2D scissorRect;
    scissorRect.offset.x = x;
    scissorRect.offset.y = y;
    scissorRect.extent.width = width;
    scissorRect.extent.height = height;
    vkCmdSetScissor(buffer, 0, 1, &scissorRect);
}

void VulkanCommandBuffer::Draw(uint32 vertexOffset, uint32 vertexCount, uint32 instanceCount)
{
    vkCmdDraw(buffer, vertexCount, instanceCount, vertexOffset, 0);
}

}
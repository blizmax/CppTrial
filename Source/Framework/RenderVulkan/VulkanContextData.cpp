#include "RenderVulkan/VulkanContextData.h"
#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/VulkanGpuQueue.h"

namespace RenderCore
{

SPtr<VulkanContextData> VulkanContextData::Create(const SPtr<GpuQueue> &gpuQueue)
{
    return Memory::MakeShared<VulkanContextData>(gpuQueue);
}

VulkanContextData::VulkanContextData(const SPtr<GpuQueue> &gpuQueue)
{
    queue = static_cast<VulkanGpuQueue *>(gpuQueue.get());
    fence = GpuFence::Create();

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = gVulkanDevice->GetQueueFamilyIndex(queue->GetQueueType());

    if(vkCreateCommandPool(gVulkanDevice->GetLogicalDeviceHandle(), &poolInfo, gVulkanAlloc, &pool) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create command pool failed.");

    commandPool = FencedPool<VkCommandBuffer>::Create(fence, [this]() {
        VkCommandBufferAllocateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.commandPool = this->pool;
        info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        info.commandBufferCount = 1;
        VkCommandBuffer buf;
        if(vkAllocateCommandBuffers(gVulkanDevice->GetLogicalDeviceHandle(), &info, &buf) != VK_SUCCESS)
            CT_EXCEPTION(RenderCore, "Create command buffer failed.");
        return buf;
    });

    buffer = commandPool->NewObject();
    InitCommandBuffer();
}

VulkanContextData::~VulkanContextData()
{
    if(pool != VK_NULL_HANDLE)
    {
        if(gVulkanDevice)
        {
            gVulkanDevice->Release([pool = pool]() {
                vkDestroyCommandPool(gVulkanDevice->GetLogicalDeviceHandle(), pool, gVulkanAlloc);
            });
        }
        pool = VK_NULL_HANDLE;
    }
}

void VulkanContextData::Flush()
{
    recording = false;
    if (vkEndCommandBuffer(buffer) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "End command buffer failed.");

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;
    if (vkQueueSubmit(queue->GetHandle(), 1, &submitInfo, nullptr) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Submit failed.");

    fence->GpuSignal(queue);

    buffer = commandPool->NewObject();
    InitCommandBuffer();
}

void VulkanContextData::InitCommandBuffer()
{
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;
    if (vkBeginCommandBuffer(buffer, &beginInfo) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Begin command buffer failed.");

    recording = true;
}

}
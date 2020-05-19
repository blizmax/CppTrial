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

    //TODO
    //Init buffer
}

VulkanContextData::~VulkanContextData()
{
    if(pool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(gVulkanDevice->GetLogicalDeviceHandle(), pool, gVulkanAlloc);
        pool = VK_NULL_HANDLE;
    }
}

void VulkanContextData::Flush()
{
    recording = false;
    vkEndCommandBuffer(buffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;
    vkQueueSubmit(queue->GetHandle(), 1, &submitInfo, nullptr);

    fence->GpuSignal(queue);

    // TODO
    // mpList = mpApiData->pCmdBufferAllocator->newObject();
    // initCommandList(mpApiData, mpList);

}

}
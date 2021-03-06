#pragma once

#include "RenderCore/FencedPool.h"
#include "RenderCore/GpuFence.h"
#include "RenderVulkan/VulkanGpuQueue.h"
#include "RenderVulkan/VulkanMemory.h"

class VulkanContextData
{
public:
    VulkanContextData(const SPtr<GpuQueue> &gpuQueue);
    ~VulkanContextData();

    void Flush();

    VkCommandBuffer GetCommandBufferHandle() const
    {
        return buffer;
    }

    VulkanGpuQueue *GetQueue() const
    {
        return queue;
    }

    const SPtr<GpuFence> &GetFence() const
    {
        return fence;
    }

    static SPtr<VulkanContextData> Create(const SPtr<GpuQueue> &gpuQueue);

private:
    void InitCommandBuffer();

private:
    VulkanGpuQueue *queue;
    SPtr<GpuFence> fence;
    VkCommandPool pool = VK_NULL_HANDLE;
    VkCommandBuffer buffer = VK_NULL_HANDLE;
    SPtr<FencedPool<VkCommandBuffer>> commandPool;
    bool recording = false;
};
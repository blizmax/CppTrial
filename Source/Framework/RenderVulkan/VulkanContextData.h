#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderVulkan/VulkanGpuQueue.h"
#include "RenderCore/GpuFence.h"

namespace RenderCore
{
class VulkanContextData
{
public:
    VulkanContextData(const SPtr<GpuQueue> &gpuQueue);
    ~VulkanContextData();

    void Flush();

    static SPtr<VulkanContextData> Create(const SPtr<GpuQueue> &gpuQueue);

private:
    VulkanGpuQueue *queue;
    SPtr<GpuFence> fence;
    VkCommandPool pool = VK_NULL_HANDLE;
    VkCommandBuffer buffer;
    bool recording = false;
};
}
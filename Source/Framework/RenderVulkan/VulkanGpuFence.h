#pragma once

#include "RenderVulkan/VulkanSync.h"
#include "RenderCore/GpuFence.h"
#include "Core/List.h"

namespace RenderCore
{

class VulkanGpuFence : public GpuFence
{
public:
    virtual ~VulkanGpuFence();

    virtual uint64 GpuSignal(GpuQueue *queue) override;
    virtual void SyncGpu(GpuQueue *queue) override;
    virtual void SyncCpu() override;

    virtual uint64 GetGpuValue() override;

    virtual uint64 GetCpuValue() override
    {
        return cpuValue;
    }

private:
    void ReleaseSemaphores();
    const SPtr<VulkanFence> &GetVulkanFence();
    const SPtr<VulkanSemaphore> &GetVulkanSemaphore();

private:
    List<SPtr<VulkanFence>> activeFences;
    Array<SPtr<VulkanFence>> freeFences;
    List<SPtr<VulkanSemaphore>> activeSemaphores;
    Array<SPtr<VulkanSemaphore>> freeSemaphores;
    Array<VkSemaphore> waitSemaphores;

    uint64 cpuValue = 1;
    uint64 gpuValue = 0;
};

}
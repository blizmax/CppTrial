#pragma once

#include "Core/List.h"
#include "RenderCore/GpuFence.h"
#include "RenderVulkan/VulkanSync.h"

class VulkanGpuFence : public GpuFence
{
public:
    virtual ~VulkanGpuFence();

    virtual uint64 GpuSignal(const GpuQueue *queue) override;
    virtual void SyncGpu(const GpuQueue *queue) override;
    virtual void SyncCpu(uint64 val) override;

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
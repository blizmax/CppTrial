#pragma once

#include "RenderCore/GpuTimer.h"
#include "RenderVulkan/VulkanMemory.h"

class VulkanGpuTimer : public GpuTimer
{
public:
    VulkanGpuTimer();
    ~VulkanGpuTimer();

    virtual void Start() override;
    virtual void Stop() override;
    virtual float GetElapsedMs() override;
};
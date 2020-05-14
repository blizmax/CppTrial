#pragma once

#include "RenderVulkan/VulkanSync.h"
#include "RenderCore/GpuFence.h"

namespace RenderCore
{

class VulkanGpuFence : public GpuFence
{
public:
    virtual ~VulkanGpuFence();

};

}
#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

class VulkanFrameBuffer : public IVulkanResource
{
public:
    VulkanFrameBuffer();

    virtual void Destroy() override;

    static SPtr<VulkanFrameBuffer> Create();

private:

};
}
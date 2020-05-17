#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/DescriptorPool.h"

namespace RenderCore
{

class VulkanDescriptorPool : public DescriptorPool
{
public:
    VulkanDescriptorPool(const DescriptorPoolDesc &desc, const SPtr<GpuFence> &fence);
    virtual ~VulkanDescriptorPool();

    VkDescriptorPool GetHandle() const
    {
        return pool;
    }

private:
    VkDescriptorPool pool = VK_NULL_HANDLE;
};

}
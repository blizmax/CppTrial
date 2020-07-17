#pragma once

#include "RenderCore/DescriptorPool.h"
#include "RenderVulkan/VulkanMemory.h"

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
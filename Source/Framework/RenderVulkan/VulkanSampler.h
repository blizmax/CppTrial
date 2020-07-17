#pragma once

#include "RenderCore/Sampler.h"
#include "RenderVulkan/VulkanMemory.h"

class VulkanSampler : public Sampler
{
public:
    VulkanSampler(const SamplerDesc &desc);
    virtual ~VulkanSampler();

    VkSampler GetHandle() const
    {
        return sampler;
    }

private:
    VkSampler sampler = VK_NULL_HANDLE;
};
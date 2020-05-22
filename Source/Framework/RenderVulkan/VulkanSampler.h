#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/Sampler.h"

namespace RenderCore
{
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


}
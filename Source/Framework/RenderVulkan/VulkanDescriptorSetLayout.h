#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

struct VulkanDescriptorSetLayoutCreateParams
{
    Array<VkDescriptorSetLayoutBinding> bindings;
};

class VulkanDescriptorSetLayout : public IVulkanResource
{
public:
    VulkanDescriptorSetLayout(const VulkanDescriptorSetLayoutCreateParams &params);

    virtual void Destroy();

    VkDescriptorSetLayout GetHandle() const
    {
        return descriptorSetLayout;
    }

private:
    Array<VkDescriptorSetLayoutBinding> bindings;
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
};

}
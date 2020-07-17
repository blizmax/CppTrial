#pragma once

#include "RenderCore/DescriptorSetLayout.h"
#include "RenderVulkan/VulkanMemory.h"

class VulkanDescriptorSetLayout : public DescriptorSetLayout
{
public:
    VulkanDescriptorSetLayout(const DescriptorSetLayoutDesc &desc);
    virtual ~VulkanDescriptorSetLayout();

    VkDescriptorSetLayout GetHandle() const
    {
        return layout;
    }

private:
    VkDescriptorSetLayout layout = VK_NULL_HANDLE;
};
#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/DescriptorSetLayout.h"

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
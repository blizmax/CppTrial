#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/DescriptorSetLayout.h"

namespace RenderCore
{

class VulkanDescriptorSetLayout : public DescriptorSetLayout
{
public:
    using Element = DescriptorSetLayout::Element;

    VulkanDescriptorSetLayout(std::initializer_list<Element> initList, ShaderVisibilityFlags visibility);
    virtual ~VulkanDescriptorSetLayout();

    VkDescriptorSetLayout GetHandle() const
    {
        return layout;
    }

private:    
    VkDescriptorSetLayout layout = VK_NULL_HANDLE;
};

}

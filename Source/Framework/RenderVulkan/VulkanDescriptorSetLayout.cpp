#include "RenderVulkan/VulkanDescriptorSetLayout.h"
#include "RenderVulkan/VulkanDevice.h"

namespace RenderCore
{

SPtr<DescriptorSetLayout> DescriptorSetLayout::Create(std::initializer_list<Element> initList, ShaderVisibilityFlags visibility)
{
    return Memory::MakeShared<VulkanDescriptorSetLayout>(initList, visibility);
}

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(std::initializer_list<Element> initList, ShaderVisibilityFlags visibility)
    : DescriptorSetLayout(initList, visibility)
{
    Array<VkDescriptorSetLayoutBinding> bindings;
    for (int32 i = 0; i < elements.Count(); ++i)
    {
        VkDescriptorSetLayoutBinding b = {};
        b.binding = i;
        b.descriptorCount = 1;
        b.descriptorType = ToVkDescriptorType(elements[i].descriptorType);
        b.pImmutableSamplers = nullptr;
        b.stageFlags = ToVkShaderVisibility(visibility);
        bindings.Add(b);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindings.Count();
    layoutInfo.pBindings = bindings.GetData();

    if (vkCreateDescriptorSetLayout(gVulkanDevice->GetLogicalDeviceHandle(), &layoutInfo, gVulkanAlloc, &layout) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create descriptor set layout failed.");
}

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
{
    if(layout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(gVulkanDevice->GetLogicalDeviceHandle(), layout, gVulkanAlloc);
        layout = VK_NULL_HANDLE;
    }
}

}
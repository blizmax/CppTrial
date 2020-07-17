#include "RenderVulkan/VulkanDescriptorSetLayout.h"
#include "RenderVulkan/VulkanDevice.h"

SPtr<DescriptorSetLayout> DescriptorSetLayout::Create(const DescriptorSetLayoutDesc &desc)
{
    return Memory::MakeShared<VulkanDescriptorSetLayout>(desc);
}

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const DescriptorSetLayoutDesc &desc)
    : DescriptorSetLayout(desc)
{
    Array<VkDescriptorSetLayoutBinding> bindings;
    for (const auto &e : desc.elements)
    {
        if (e.descriptorType != DescriptorType::Unknown)
        {
            VkDescriptorSetLayoutBinding b = {};
            b.binding = e.binding;
            b.descriptorCount = e.arrayLength;
            b.descriptorType = ToVkDescriptorType(e.descriptorType);
            b.pImmutableSamplers = nullptr;
            b.stageFlags = ToVkShaderVisibility(desc.visibility);
            bindings.Add(b);
        }
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
    if (layout != VK_NULL_HANDLE)
    {
        if (gVulkanDevice)
        {
            gVulkanDevice->Release([layout = layout]() {
                vkDestroyDescriptorSetLayout(gVulkanDevice->GetLogicalDeviceHandle(), layout, gVulkanAlloc);
            });
        }
        layout = VK_NULL_HANDLE;
    }
}
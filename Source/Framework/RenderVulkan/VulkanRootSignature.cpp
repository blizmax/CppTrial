#include "RenderVulkan/VulkanRootSignature.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{

VulkanRootSignature::VulkanRootSignature(const RootSignatureDesc &desc) : RootSignature(desc)
{
    auto CreateDescriptorSetLayout = [](const SPtr<DescriptorSetLayout> &layout)
    {
        Array<VkDescriptorSetLayoutBinding> bindings;
        const auto &elements = layout->GetElements();
        for (int32 i = 0; i < elements.Count(); ++i)
        {
            VkDescriptorSetLayoutBinding b = {};
            b.binding = i;
            b.descriptorCount = 1;
            b.descriptorType = ToVkDescriptorType(elements[i].descriptorType);
            b.pImmutableSamplers = nullptr;
            b.stageFlags = ToVkShaderVisibility(layout->GetVisibility());
            bindings.Add(b);
        }

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = bindings.Count();
        layoutInfo.pBindings = bindings.GetData();

        VkDescriptorSetLayout vkHandle;
        if (vkCreateDescriptorSetLayout(gVulkanContext->GetLogicalDeviceHandle(), &layoutInfo, gVulkanAlloc, &vkHandle) != VK_SUCCESS)
            CT_EXCEPTION(RenderCore, "Create descriptor set layout failed.");
        
        return vkHandle;
    };

    for(auto &e : desc.layouts)
    {
        setLayouts.Add(CreateDescriptorSetLayout(e));
    }

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = setLayouts.Count();
    pipelineLayoutInfo.pSetLayouts = setLayouts.GetData();
    if(vkCreatePipelineLayout(gVulkanContext->GetLogicalDeviceHandle(), &pipelineLayoutInfo, gVulkanAlloc, &pipelineLayout) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create pipeline layout failed.");
}

VulkanRootSignature::~VulkanRootSignature()
{
    for (auto& e : setLayouts)
    {
        vkDestroyDescriptorSetLayout(gVulkanContext->GetLogicalDeviceHandle(), e, gVulkanAlloc);
    }
    setLayouts.Clear();

    if(pipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(gVulkanContext->GetLogicalDeviceHandle(), pipelineLayout, gVulkanAlloc);
        pipelineLayout = VK_NULL_HANDLE;
    }
}

}
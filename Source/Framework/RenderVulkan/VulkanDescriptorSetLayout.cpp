#include "RenderVulkan/VulkanDescriptorSetLayout.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const VulkanDescriptorSetLayoutCreateParams &params)
    :bindings(params.bindings)
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.pNext = nullptr;
    layoutInfo.flags = 0;
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindings.Count();
    layoutInfo.pBindings = bindings.GetData();

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, gVulkanAlloc, &descriptorSetLayout) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create descriptor set layout failed.");
}

void VulkanDescriptorSetLayout::Destroy()
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();

    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, gVulkanAlloc);
}

}
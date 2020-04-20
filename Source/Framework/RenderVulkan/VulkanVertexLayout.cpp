#include "RenderVulkan/VulkanVertexLayout.h"

namespace RenderCore
{
SPtr<VertexLayout> VertexLayout::Create(const VertexLayoutCreateParams &params)
{
    return Memory::MakeShared<VulkanVertexLayout>(params);
}

VulkanVertexLayout::VulkanVertexLayout(const VertexLayoutCreateParams &params)
    : stride(params.stride), attributes(params.attributes)
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = stride;
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    Array<VkVertexInputAttributeDescription> attributeDescs;
    int32 location = 0;
    for(auto &e : attributes)
    {
        VkVertexInputAttributeDescription attrib = {};
        attrib.location = location++;
        attrib.format = ToVkDataFormat(e.dataType);
        attrib.offset = e.offset;
        attrib.binding = 0;

        attributeDescs.Add(attrib);
    }

    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.pVertexBindingDescriptions = &bindingDescription;
    createInfo.vertexBindingDescriptionCount = 1;
    createInfo.pVertexAttributeDescriptions = attributeDescs.GetData();
    createInfo.vertexAttributeDescriptionCount = attributeDescs.Count();
}
}
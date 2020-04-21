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
    bindingDesc = {};
    bindingDesc.binding = 0;
    bindingDesc.stride = stride;
    bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

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
    createInfo.pVertexBindingDescriptions = &bindingDesc;
    createInfo.vertexBindingDescriptionCount = 1;
    createInfo.pVertexAttributeDescriptions = attributeDescs.GetData();
    createInfo.vertexAttributeDescriptionCount = attributeDescs.Count();
}
}
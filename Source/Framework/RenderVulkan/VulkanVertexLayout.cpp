#include "RenderVulkan/VulkanVertexLayout.h"

namespace RenderCore
{
SPtr<VertexLayout> VertexLayout::Create()
{
    return Memory::MakeShared<VulkanVertexLayout>();
}

VulkanVertexLayout::VulkanVertexLayout()
{
    for(int32 i = 0; i < bufferLayouts.Count(); ++i)
    {
        VkVertexInputBindingDescription bindingDesc = {};
        bindingDesc.binding = i;
        bindingDesc.stride = bufferLayouts[i]->GetStride();
        bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescs.Add(bindingDesc);

        int32 location = 0;
        for(const auto &e : bufferLayouts[i]->GetElements())
        {
            VkVertexInputAttributeDescription attrib = {};
            attrib.location = location++;
            attrib.format = ToVkResourceFormat(e.format);
            attrib.offset = e.offset;
            attrib.binding = i;

            attributeDescs.Add(attrib);
        }
    }

    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.pVertexBindingDescriptions = bindingDescs.GetData();
    createInfo.vertexBindingDescriptionCount = bindingDescs.Count();
    createInfo.pVertexAttributeDescriptions = attributeDescs.GetData();
    createInfo.vertexAttributeDescriptionCount = attributeDescs.Count();
}
}
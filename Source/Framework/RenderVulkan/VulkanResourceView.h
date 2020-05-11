#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/ResourceView.h"

namespace RenderCore
{
class VulkanBufferView : public ResourceView
{
public:
    VulkanBufferView(const WPtr<Resource> &resource, uint32 firstElement, uint32 elementCount);
    virtual ~VulkanBufferView();

private:
    VkBufferView bufferView = VK_NULL_HANDLE;
};

}
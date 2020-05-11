#include "RenderVulkan/VulkanResourceView.h"
#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/VulkanBuffer.h"

namespace RenderCore
{

SPtr<ResourceView> CreateSrv(const SPtr<Buffer> &buffer, uint32 first, uint32 count)
{
    return Memory::MakeShared<VulkanBufferView>(buffer, first, count);
}

SPtr<ResourceView> CreateUav(const SPtr<Buffer> &buffer, uint32 first, uint32 count)
{
    return Memory::MakeShared<VulkanBufferView>(buffer, first, count);
}

SPtr<ResourceView> CreateCbv(const SPtr<Buffer> &buffer)
{
    return Memory::MakeShared<VulkanBufferView>(buffer, 0, 1);
}

VulkanBufferView::VulkanBufferView(const WPtr<Resource> &resource, uint32 firstElement, uint32 elementCount)
    : ResourceView(resource, firstElement, elementCount)
{
    auto buffer = dynamic_cast<VulkanBuffer *>(GetResource());
    if(buffer && buffer->IsTyped())
    {
        VkBufferViewCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
        bufferInfo.buffer = buffer->GetHandle();
        bufferInfo.offset = 0;
        bufferInfo.range = VK_WHOLE_SIZE;
        bufferInfo.format =  ToVkResourceFormat(buffer->GetResourceFormat());

        if(vkCreateBufferView(gVulkanContext->GetLogicalDeviceHandle(), &bufferInfo, gVulkanAlloc, &bufferView) != VK_SUCCESS)
            CT_EXCEPTION(RenderCore, "Create buffer view failed.");
    }
}

VulkanBufferView::~VulkanBufferView()
{
    if(bufferView != VK_NULL_HANDLE)
    {
        vkDestroyBufferView(gVulkanContext->GetLogicalDeviceHandle(), bufferView, gVulkanAlloc);
        bufferView = VK_NULL_HANDLE;
    }
}
}
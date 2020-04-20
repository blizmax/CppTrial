#include "RenderVulkan/VulkanBuffer.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
VulkanBuffer::VulkanBuffer(const VulkanBufferCreateParams &params)
{
    auto device = VulkanContext::Get().GetDevice();

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.flags = 0;
    bufferInfo.size = params.size;
    bufferInfo.usage = params.usageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 0;
	bufferInfo.pQueueFamilyIndices = nullptr;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    //allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    
    vmaCreateBuffer(device->GetAllocator(), &bufferInfo, &allocInfo, &buffer, &allocation, nullptr);
}        

void VulkanBuffer::Destroy()
{
    if(allocation != VK_NULL_HANDLE)
    {
        auto device = VulkanContext::Get().GetDevice();
        vmaDestroyBuffer(device->GetAllocator(), buffer, allocation);

        allocation = VK_NULL_HANDLE;
        buffer = VK_NULL_HANDLE;
    }
}

}
#include "RenderVulkan/VulkanBuffer.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
void VulkanBuffer::Init(uint32 size, VkBufferUsageFlags usage)
{
    auto allocator = VulkanContext::Get().GetVmaAllocator();

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.flags = 0;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 0;
	bufferInfo.pQueueFamilyIndices = nullptr;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    
    vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &buffer, &allocation, nullptr);
}        

void VulkanBuffer::Destroy()
{
    if(allocation != VK_NULL_HANDLE)
    {
        auto allocator = VulkanContext::Get().GetVmaAllocator();
        vmaDestroyBuffer(allocator, buffer, allocation);

        allocation = VK_NULL_HANDLE;
        buffer = VK_NULL_HANDLE;
    }
}

void *VulkanBuffer::Map()
{
    auto allocator = VulkanContext::Get().GetVmaAllocator();

    void *data = nullptr;
    vmaMapMemory(allocator, allocation, &data);
    return data;
}

void VulkanBuffer::Unmap()
{
    auto allocator = VulkanContext::Get().GetVmaAllocator();

    vmaUnmapMemory(allocator, allocation);
}

}
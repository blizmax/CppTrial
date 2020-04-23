#include "RenderVulkan/VulkanBuffer.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
void VulkanBuffer::Init(const VulkanBufferCreateParams &params)
{
    size = params.size;
    bufferType = params.bufferType;
    usage = params.usage;
    useStaging = (params.usage != GpuBufferUsage::Dynamic);
 
    if(useStaging)
        CT_EXCEPTION(RenderCore, "Not implement.");

    bufferData = CreateBuffer(false);
}        

void VulkanBuffer::Destroy()
{
    DestroyBuffer(bufferData);
}

void *VulkanBuffer::Map()
{
    auto allocator = VulkanContext::Get().GetVmaAllocator();

    void *data = nullptr;
    vmaMapMemory(allocator, bufferData.allocation, &data);
    return data;
}

void VulkanBuffer::Unmap()
{
    auto allocator = VulkanContext::Get().GetVmaAllocator();

    vmaUnmapMemory(allocator, bufferData.allocation);
}

VulkanBuffer::BufferData VulkanBuffer::CreateBuffer(bool staging)
{
    auto allocator = VulkanContext::Get().GetVmaAllocator();

    VkBufferUsageFlags usageFlags = 0;

    if(staging)
    {
        usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    }
    else
    {
        switch(bufferType)
        {
        case VulkanBufferType::Vertex:
            usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            break;
        case VulkanBufferType::Index:
            usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            break;
        case VulkanBufferType::Uniform:
            usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            break;
        }
    }

    VkMemoryPropertyFlags requiredFlags = 0;
    if(useStaging && !staging)
    {
        requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }
    else
    {
        requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.flags = 0;
    bufferInfo.size = size;
    bufferInfo.usage = usageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 0;
	bufferInfo.pQueueFamilyIndices = nullptr;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    
    BufferData result;
    vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &result.buffer, &result.allocation, nullptr);
    return result;
}

void VulkanBuffer::DestroyBuffer(BufferData &data)
{
    if(data.allocation != VK_NULL_HANDLE)
    {
        auto allocator = VulkanContext::Get().GetVmaAllocator();
        vmaDestroyBuffer(allocator, data.buffer, data.allocation);

        data.allocation = VK_NULL_HANDLE;
        data.buffer = VK_NULL_HANDLE;
    }
}

}
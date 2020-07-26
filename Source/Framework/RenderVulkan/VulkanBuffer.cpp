#include "RenderVulkan/VulkanBuffer.h"
#include "RenderVulkan/VulkanDevice.h"

SPtr<Buffer> Buffer::Create(uint32 size, ResourceBindFlags bindFlags, BufferCpuAccess access, const void *data)
{
    auto ptr = Memory::MakeShared<VulkanBuffer>(size, bindFlags, access);
    ptr->weakThis = ptr;
    if (data)
        ptr->SetBlob(data, 0, size);
    return ptr;
}

VulkanBuffer::VulkanBuffer(uint32 size, ResourceBindFlags bindFlags, BufferCpuAccess access)
    : Buffer(size, bindFlags, access)
{
    if (access == BufferCpuAccess::Write)
    {
        bufferData = CreateBuffer(bindFlags, MemoryUsage::Upload);
    }
    else
    {
        if (access == BufferCpuAccess::Read && bindFlags == ResourceBind::None)
            bufferData = CreateBuffer(bindFlags, MemoryUsage::Download);
        else
            bufferData = CreateBuffer(bindFlags, MemoryUsage::Default);
    }
}

VulkanBuffer::~VulkanBuffer()
{
    CT_CHECK(!stagingBuffer);

    DestroyBuffer(bufferData);
}

VulkanBuffer::BufferData VulkanBuffer::CreateBuffer(ResourceBindFlags bindFlags, MemoryUsage memoryUsage)
{
    auto allocator = gVulkanDevice->GetVmaAllocator();

    VkBufferUsageFlags usageFlags = ToVkBufferUsage(bindFlags);
    usageFlags |= (VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.flags = 0;
    bufferInfo.size = size;
    bufferInfo.usage = usageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 0;
    bufferInfo.pQueueFamilyIndices = nullptr;

    VkMemoryPropertyFlags requiredFlags = ToVkMemoryProperty(memoryUsage);

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    allocInfo.requiredFlags = requiredFlags;

    BufferData result;
    if (vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &result.buffer, &result.allocation, nullptr) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create buffer failed.");
    return result;
}

void VulkanBuffer::DestroyBuffer(BufferData &data)
{
    if (data.allocation != VK_NULL_HANDLE)
    {
        if (gVulkanDevice)
        {
            gVulkanDevice->Release([buffer = data.buffer, allocation = data.allocation]() {
                auto allocator = gVulkanDevice->GetVmaAllocator();
                vmaDestroyBuffer(allocator, buffer, allocation);
            });
        }
        data.allocation = VK_NULL_HANDLE;
        data.buffer = VK_NULL_HANDLE;
    }
}

void *VulkanBuffer::Map(BufferMapType mapType)
{
    if (mapType == BufferMapType::Write || mapType == BufferMapType::WriteDiscard)
    {
        CT_CHECK(cpuAccess == BufferCpuAccess::Write);
    }

    if (mapType == BufferMapType::WriteDiscard)
    {
        ClearViews();
        DestroyBuffer(bufferData);
        bufferData = CreateBuffer(bindFlags, MemoryUsage::Upload);
    }

    bool useStaging = false;
    if (mapType == BufferMapType::Read)
    {
        if ((cpuAccess == BufferCpuAccess::Read && bindFlags != ResourceBind::None) || cpuAccess == BufferCpuAccess::None)
            useStaging = true;
    }
    if (useStaging)
    {
        if (!stagingBuffer)
        {
            stagingBuffer = Buffer::Create(size, ResourceBind::None, BufferCpuAccess::Read, nullptr);
        }

        auto context = gVulkanDevice->GetRenderContext();
        context->CopyResource(stagingBuffer.get(), this);
        context->Flush(true);
        return stagingBuffer->Map(BufferMapType::Read);
    }
    else
    {
        void *data = nullptr;
        vmaMapMemory(gVulkanDevice->GetVmaAllocator(), bufferData.allocation, &data);
        return data;
    }
}

void VulkanBuffer::Unmap()
{
    if (stagingBuffer)
    {
        stagingBuffer->Unmap();
        stagingBuffer = nullptr;
    }
    else
    {
        vmaUnmapMemory(gVulkanDevice->GetVmaAllocator(), bufferData.allocation);
    }
}

void VulkanBuffer::SetBlob(const void *data, uint32 offset, uint32 dataSize)
{
    CT_CHECK(offset + dataSize <= size);

    if (cpuAccess == BufferCpuAccess::Write)
    {
        uint8 *dst = reinterpret_cast<uint8 *>(Map(BufferMapType::WriteDiscard));
        std::memcpy(dst + offset, data, dataSize);
        Unmap();
    }
    else
    {
        auto context = gVulkanDevice->GetRenderContext();
        context->UpdateBuffer(this, data, offset, dataSize);
    }
}
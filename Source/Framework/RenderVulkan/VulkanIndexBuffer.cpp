#include "RenderVulkan/VulkanIndexBuffer.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
SPtr<IndexBuffer> IndexBuffer::Create(const IndexBufferCreateParams &params)
{
    auto result = Memory::MakeShared<VulkanIndexBuffer>(params);
    VulkanContext::Get().GetIndexBufferRegistry().Register(result.get());
    return result;
}

VulkanIndexBuffer::VulkanIndexBuffer(const IndexBufferCreateParams &params)
{
    VulkanBufferCreateParams bufferParams;
    bufferParams.bufferType = VulkanBufferType::Index;
    bufferParams.usage = params.usage;
    bufferParams.size = params.indexCount * sizeof(uint32);
    buffer.Init(bufferParams);
}

VulkanIndexBuffer::~VulkanIndexBuffer()
{
    VulkanContext::Get().GetIndexBufferRegistry().Remove(this);
}

void *VulkanIndexBuffer::Map()
{
    return buffer.Map();
}

void VulkanIndexBuffer::Unmap()
{
    buffer.Unmap();
}

void VulkanIndexBuffer::Destroy()
{
    buffer.Destroy();
}

}
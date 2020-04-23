#include "RenderVulkan/VulkanVertexBuffer.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
SPtr<VertexBuffer> VertexBuffer::Create(const VertexBufferCreateParams &params)
{
    auto result = Memory::MakeShared<VulkanVertexBuffer>(params);
    VulkanContext::Get().GetVertexBufferRegistry().Register(result.get());
    return result;
}

VulkanVertexBuffer::VulkanVertexBuffer(const VertexBufferCreateParams &params)
{
    VulkanBufferCreateParams bufferParams;
    bufferParams.bufferType = VulkanBufferType::Vertex;
    bufferParams.usage = params.usage;
    bufferParams.size = params.size;
    buffer.Init(bufferParams);
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
    VulkanContext::Get().GetVertexBufferRegistry().Remove(this);
}

void *VulkanVertexBuffer::Map()
{
    return buffer.Map();
}

void VulkanVertexBuffer::Unmap()
{
    buffer.Unmap();
}

void VulkanVertexBuffer::Destroy()
{
    buffer.Destroy();
}

}
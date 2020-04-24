#include "RenderVulkan/VulkanUniformBuffer.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
SPtr<UniformBuffer> UniformBuffer::Create(const UniformBufferCreateParams &params)
{
    auto result = Memory::MakeShared<VulkanUniformBuffer>(params);
    VulkanContext::Get().GetUniformBufferRegistry().Register(result.get());
    return result;
}

VulkanUniformBuffer::VulkanUniformBuffer(const UniformBufferCreateParams &params)
{
    VulkanBufferCreateParams bufferParams;
    bufferParams.bufferType = VulkanBufferType::Uniform;
    bufferParams.usage = params.usage;
    bufferParams.size = params.size;
    buffer.Init(bufferParams);
}

VulkanUniformBuffer::~VulkanUniformBuffer()
{
    VulkanContext::Get().GetUniformBufferRegistry().Remove(this);
}

void *VulkanUniformBuffer::Map()
{
    return buffer.Map();
}

void VulkanUniformBuffer::Unmap()
{
    buffer.Unmap();
}

void VulkanUniformBuffer::Destroy()
{
    buffer.Destroy();
}

}
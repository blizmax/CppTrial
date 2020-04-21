#include "RenderVulkan/VulkanRenderAPI.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{

RenderAPI &RenderAPI::Get()
{
    static VulkanRenderAPI instance;
    return instance;
}

void VulkanRenderAPI::Init()
{
    VulkanContext::Get().Init();
}

void VulkanRenderAPI::Destroy()
{
    VulkanContext::Get().Destroy();
}

void VulkanRenderAPI::SetViewport(int32 x, int32 y, uint32 width, uint32 height)
{

}

void VulkanRenderAPI::SetScissor(int32 x, int32 y, uint32 width, uint32 height)
{

}

void VulkanRenderAPI::SetStencilRef(uint32 value)
{

}

void VulkanRenderAPI::SetRenderPipeline(const SPtr<RenderPipelineState> &pipeline)
{

}

void VulkanRenderAPI::SetVertexBuffers(uint32 startIndex, SPtr<VertexBuffer> *buffers, uint32 count)
{

}

void VulkanRenderAPI::SetIndexBuffer(const SPtr<IndexBuffer> &buffer)
{

}

}
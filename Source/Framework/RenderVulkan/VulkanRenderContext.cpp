#include "RenderVulkan/VulkanRenderContext.h"
#include "RenderVulkan/VulkanBuffer.h"
#include "RenderVulkan/VulkanTexture.h"

namespace RenderCore
{

SPtr<RenderContext> RenderContext::Create(const SPtr<GpuQueue> &queue)
{
    return Memory::MakeShared<VulkanRenderContext>(queue);
}

VulkanRenderContext::VulkanRenderContext(const SPtr<GpuQueue> &queue)
    : VulkanComputeContext(queue)
{

}

VulkanRenderContext::~VulkanRenderContext()
{
}

void VulkanRenderContext::ClearFrameBuffer(const FrameBuffer *fbo, const Color &color, float depth, uint8 stencil, bool clearColor, bool clearDepthStencil)
{
    bool hasDepthStencilTexture = fbo->GetDepthStencilTexture() != nullptr;
    auto depthStencilFormat = hasDepthStencilTexture ? fbo->GetDepthStencilTexture()->GetResourceFormat() : ResourceFormat::Unknown;

    if (clearColor)
    {
        for (int32 i = 0; i < COLOR_ATTCHMENT_MAX_NUM; ++i)
        {
            if (fbo->GetColorTexture(i))
            {
                ClearRtv(fbo->GetRtv(i).get(), color);
            }
        }
    }

    if (clearDepthStencil)
    {
        ClearDsv(fbo->GetDsv().get(), depth, stencil, true, true);
    }
}

void VulkanRenderContext::ClearRtv(const ResourceView *rtv, const Color &color)
{
    ClearColorImage(rtv, color.r, color.g, color.b, color.a);
    commandsPending = true;
}

void VulkanRenderContext::ClearDsv(const ResourceView *dsv, float depth, uint8 stencil, bool clearDepth, bool clearStencil)
{
    auto vkTexture = dynamic_cast<const VulkanTexture *>(dsv->GetResource());
    CT_CHECK(vkTexture != nullptr);

    VulkanCopyContext::ResourceBarrier(vkTexture, ResourceState::CopyDest, nullptr);
    VkClearDepthStencilValue dsVal = {};
    dsVal.depth = depth;
    dsVal.stencil = stencil;

    VkImageSubresourceRange range = {};
    const auto& viewInfo = dsv->GetViewInfo();
    range.baseArrayLayer = viewInfo.firstArraySlice;
    range.baseMipLevel = viewInfo.mostDetailedMip;
    range.layerCount = viewInfo.arrayLayers;
    range.levelCount = viewInfo.mipLevels;
    range.aspectMask = clearDepth ? VK_IMAGE_ASPECT_DEPTH_BIT : 0;
    range.aspectMask |= clearStencil ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;

    vkCmdClearDepthStencilImage(contextData->GetCommandBufferHandle(), vkTexture->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &dsVal, 1, &range);
    
    commandsPending = true;
}

void VulkanRenderContext::ClearTexture(Texture *texture, const Color &color)
{
    CT_CHECK(texture != nullptr);

    auto format = texture->GetResourceFormat();
    auto componentType = GetResourceComponentType(format);
    if (componentType == ResourceComponentType::Int || componentType == ResourceComponentType::UInt || componentType == ResourceComponentType::Unknown)
    {
        CT_LOG(Warning, CT_TEXT("Unsupported texture format, format: {0}."), (int32)format);
        return;
    }

    auto bindFlags = texture->GetBindFlags();
    if(bindFlags & ResourceBind::RenderTarget != 0)
    {
        ClearRtv(texture->GetRtv().get(), color);
    }
    else if(bindFlags & ResourceBind::DepthStencil != 0)
    {
        ClearDsv(texture->GetDsv().get(), color.r, 0, true, true);
    }
    else if(bindFlags & ResourceBind::UnorderedAccess != 0)
    {
        ClearColorImage(texture->GetUav().get(), color.r, color.g, color.b, color.a);
    }
    else
    {
        CT_LOG(Warning, CT_TEXT("Unsupported bind flags."));
    }
}

void VulkanRenderContext::Draw(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 firstVertex)
{
    DrawInstanced(state, vars, vertexCount, 1, firstVertex, 0);
}

void VulkanRenderContext::DrawInstanced(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance)
{
    if(PrepareForDraw(state, vars) == false)
        return;
    
    vkCmdDraw(contextData->GetCommandBufferHandle(), vertexCount, instanceCount, firstVertex, firstInstance);
    vkCmdEndRenderPass(contextData->GetCommandBufferHandle());
}

void VulkanRenderContext::DrawIndexed(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 firstIndex, int32 vertexOffset)
{
    DrawIndexedInstanced(state, vars, indexCount, 1, firstIndex, vertexOffset, 0);
}

void VulkanRenderContext::DrawIndexedInstanced(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 instanceCount, uint32 firstIndex, int32 vertexOffset, uint32 firstInstance)
{
    if(PrepareForDraw(state, vars) == false)
        return;
    
    vkCmdDrawIndexed(contextData->GetCommandBufferHandle(), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    vkCmdEndRenderPass(contextData->GetCommandBufferHandle());
}

bool VulkanRenderContext::ApplyGraphicsVars(GraphicsVars *vars, RootSignature *rootSignature)
{
    //TODO
    return true;
}

void VulkanRenderContext::SetVao(const VertexArray *vao)
{
    auto commandBuffer = contextData->GetCommandBufferHandle();
    const auto &buffers = vao->GetVertexBuffers();
    for(int32 i = 0; i < buffers.Count(); ++i)
    {
        auto vkBuffer = static_cast<VulkanBuffer *>(buffers[i].get());
        VkDeviceSize offset = vkBuffer->GetOffset();
        VkBuffer handle = vkBuffer->GetHandle();
        vkCmdBindVertexBuffers(commandBuffer, i, 1, &handle, &offset);
        VulkanCopyContext::ResourceBarrier(vkBuffer, ResourceState::VertexBuffer, nullptr);
    }
    if(vao->GetIndexBuffer())
    {
        auto vkBuffer = static_cast<VulkanBuffer *>(vao->GetIndexBuffer().get());
        VkDeviceSize offset = vkBuffer->GetOffset();
        VkBuffer handle = vkBuffer->GetHandle();
        vkCmdBindIndexBuffer(commandBuffer, handle, offset, ToVkIndexType(vkBuffer->GetResourceFormat()));
        VulkanCopyContext::ResourceBarrier(vkBuffer, ResourceState::IndexBuffer, nullptr);
    }
}

void VulkanRenderContext::SetVao(const VertexArray *vao)
{
    auto commandBuffer = contextData->GetCommandBufferHandle();
    const auto &buffers = vao->GetVertexBuffers();
    for(int32 i = 0; i < buffers.Count(); ++i)
    {
        auto vkBuffer = static_cast<VulkanBuffer *>(buffers[i].get());
        VkDeviceSize offset = vkBuffer->GetOffset();
        VkBuffer handle = vkBuffer->GetHandle();
        vkCmdBindVertexBuffers(commandBuffer, i, 1, &handle, &offset);
        VulkanCopyContext::ResourceBarrier(vkBuffer, ResourceState::VertexBuffer, nullptr);
    }
    if(vao->GetIndexBuffer())
    {
        auto vkBuffer = static_cast<VulkanBuffer *>(vao->GetIndexBuffer().get());
        VkDeviceSize offset = vkBuffer->GetOffset();
        VkBuffer handle = vkBuffer->GetHandle();
        vkCmdBindIndexBuffer(commandBuffer, handle, offset, ToVkIndexType(vkBuffer->GetResourceFormat()));
        VulkanCopyContext::ResourceBarrier(vkBuffer, ResourceState::IndexBuffer, nullptr);
    }
}

void VulkanRenderContext::SetFbo(const FrameBuffer *fbo)
{
    for(int i = 0; i < COLOR_ATTCHMENT_MAX_NUM; ++i)
    {
        const auto &texture = fbo->GetColorTexture(i);
        if(texture)
        {
            VulkanCopyContext::ResourceBarrier(texture.get(), ResourceState::RenderTarget, nullptr);
        }
    }
    if(fbo->GetDepthStencilTexture())
    {
        VulkanCopyContext::ResourceBarrier(fbo->GetDepthStencilTexture().get(), ResourceState::DepthStencil, nullptr);
    }
}

void VulkanRenderContext::SetViewports(const Array<Viewport> &viewports)
{

}

void VulkanRenderContext::SetScissors(const Array<Scissor> &scissors)
{

}

bool VulkanRenderContext::PrepareForDraw(GraphicsState *state, GraphicsVars *vars)
{
    auto gso = state->GetGso(vars);

    if(bindFlags & GraphicsStateBind::Vars)
    {
        if(vars)
        {
            if(!ApplyGraphicsVars(vars, gso->GetDesc().rootSignature.get()))
                return false;
        }
    }

    if(bindFlags & GraphicsStateBind::PipelineState)
        
    if(bindFlags & GraphicsStateBind::Vao)
        SetVao(state->GetVertexArray().get());
    if(bindFlags & GraphicsStateBind::Fbo)
        SetFbo(state->GetFrameBuffer().get());
    if(bindFlags & GraphicsStateBind::Viewports)
        SetViewports(state->GetViewports());
    if(bindFlags & GraphicsStateBind::Scissors)
        SetScissors(state->GetScissors());

    //TODO
    return true;
}

}
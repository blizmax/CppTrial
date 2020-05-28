#include "RenderVulkan/VulkanRenderContext.h"
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

bool VulkanRenderContext::PrepareForDraw(GraphicsState *state, GraphicsVars *vars)
{
    //TODO
    return true;
}

}
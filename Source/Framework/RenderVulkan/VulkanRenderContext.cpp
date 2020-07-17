#include "RenderVulkan/VulkanRenderContext.h"
#include "RenderVulkan/VulkanBuffer.h"
#include "RenderVulkan/VulkanFrameBuffer.h"
#include "RenderVulkan/VulkanGraphicsStateObject.h"
#include "RenderVulkan/VulkanTexture.h"

SPtr<RenderContext> RenderContext::Create(const SPtr<GpuQueue> &queue)
{
    return Memory::MakeShared<VulkanRenderContext>(queue);
}

VulkanRenderContextImpl::VulkanRenderContextImpl(const SPtr<GpuQueue> &queue, RenderContext *ctx)
    : VulkanComputeContextImpl(queue, ctx), renderContext(ctx)
{
}

VulkanRenderContextImpl::~VulkanRenderContextImpl()
{
}

void VulkanRenderContextImpl::ClearFrameBuffer(const FrameBuffer *fbo, const Color &color, float depth, uint8 stencil, bool clearColor, bool clearDepthStencil)
{
    bool hasDepthStencilTexture = fbo->GetDepthStencilTexture() != nullptr;
    auto depthStencilFormat = hasDepthStencilTexture ? fbo->GetDepthStencilTexture()->GetResourceFormat() : ResourceFormat::Unknown;

    if (clearColor)
    {
        for (int32 i = 0; i < fbo->GetColorTextureCount(); ++i)
        {
            ClearRtv(fbo->GetRtv(i).get(), color);
        }
    }

    if (clearDepthStencil)
    {
        ClearDsv(fbo->GetDsv().get(), depth, stencil, true, true);
    }
}

void VulkanRenderContextImpl::ClearRtv(const ResourceView *rtv, const Color &color)
{
    ClearColorImage(rtv, color.r, color.g, color.b, color.a);
    commandsPending = true;
}

void VulkanRenderContextImpl::ClearDsv(const ResourceView *dsv, float depth, uint8 stencil, bool clearDepth, bool clearStencil)
{
    auto vkTexture = dynamic_cast<const VulkanTexture *>(dsv->GetResource());
    CT_CHECK(vkTexture != nullptr);

    ResourceBarrier(vkTexture, ResourceState::CopyDest, nullptr);
    VkClearDepthStencilValue dsVal = {};
    dsVal.depth = depth;
    dsVal.stencil = stencil;

    VkImageSubresourceRange range = {};
    const auto &viewInfo = dsv->GetViewInfo();
    range.baseArrayLayer = viewInfo.firstArraySlice;
    range.baseMipLevel = viewInfo.mostDetailedMip;
    range.layerCount = viewInfo.arrayLayers;
    range.levelCount = viewInfo.mipLevels;
    range.aspectMask = clearDepth ? VK_IMAGE_ASPECT_DEPTH_BIT : 0;
    range.aspectMask |= clearStencil ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;

    vkCmdClearDepthStencilImage(contextData->GetCommandBufferHandle(), vkTexture->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &dsVal, 1, &range);

    commandsPending = true;
}

void VulkanRenderContextImpl::ClearTexture(Texture *texture, const Color &color)
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
    if (bindFlags & ResourceBind::RenderTarget)
    {
        ClearRtv(texture->GetRtv().get(), color);
    }
    else if (bindFlags & ResourceBind::DepthStencil)
    {
        ClearDsv(texture->GetDsv().get(), color.r, 0, true, true);
    }
    else if (bindFlags & ResourceBind::UnorderedAccess)
    {
        ClearColorImage(texture->GetUav().get(), color.r, color.g, color.b, color.a);
    }
    else
    {
        CT_LOG(Warning, CT_TEXT("Unsupported bind flags."));
    }
}

void VulkanRenderContextImpl::Draw(GraphicsState *state, GraphicsVars *vars, int32 vertexCount, int32 firstVertex)
{
    DrawInstanced(state, vars, vertexCount, 1, firstVertex, 0);
}

void VulkanRenderContextImpl::DrawInstanced(GraphicsState *state, GraphicsVars *vars, int32 vertexCount, int32 instanceCount, int32 firstVertex, int32 firstInstance)
{
    CT_CHECK(vertexCount >= 0 && instanceCount >= 0 && firstVertex >= 0 && firstInstance >= 0);

    if (PrepareForDraw(state, vars) == false)
        return;

    vkCmdDraw(contextData->GetCommandBufferHandle(), vertexCount, instanceCount, firstVertex, firstInstance);
    vkCmdEndRenderPass(contextData->GetCommandBufferHandle());
}

void VulkanRenderContextImpl::DrawIndexed(GraphicsState *state, GraphicsVars *vars, int32 indexCount, int32 firstIndex, int32 vertexOffset)
{
    DrawIndexedInstanced(state, vars, indexCount, 1, firstIndex, vertexOffset, 0);
}

void VulkanRenderContextImpl::DrawIndexedInstanced(GraphicsState *state, GraphicsVars *vars, int32 indexCount, int32 instanceCount, int32 firstIndex, int32 vertexOffset, int32 firstInstance)
{
    CT_CHECK(indexCount >= 0 && instanceCount >= 0 && firstIndex >= 0 && firstInstance >= 0);

    if (PrepareForDraw(state, vars) == false)
        return;

    vkCmdDrawIndexed(contextData->GetCommandBufferHandle(), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    vkCmdEndRenderPass(contextData->GetCommandBufferHandle());
}

template <int32 offsetCount, typename ViewType>
void InitBlitData(const ViewType *view, const Vector4I &rect, VkImageSubresourceLayers &layer, VkOffset3D offset[offsetCount])
{
    const Texture *texture = dynamic_cast<const Texture *>(view->GetResource());

    layer.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    const auto &viewInfo = view->GetViewInfo();
    layer.baseArrayLayer = viewInfo.firstArraySlice;
    layer.layerCount = viewInfo.arrayLayers;
    layer.mipLevel = viewInfo.mostDetailedMip;
    CT_CHECK(texture->GetDepth(viewInfo.mostDetailedMip) == 1);

    offset[0].x = (rect.x == -1) ? 0 : rect.x;
    offset[0].y = (rect.y == -1) ? 0 : rect.y;
    offset[0].z = 0;

    if (offsetCount > 1)
    {
        offset[1].x = (rect.z == -1) ? texture->GetWidth(viewInfo.mostDetailedMip) : rect.z;
        offset[1].y = (rect.w == -1) ? texture->GetHeight(viewInfo.mostDetailedMip) : rect.w;
        offset[1].z = 1;
    }
}

void VulkanRenderContextImpl::Blit(ResourceView *src, ResourceView *dst, const Vector4I &srcRect, const Vector4I &dstRect, TextureFilter filter)
{
    ResourceBarrier(src->GetResource(), ResourceState::CopySource, &src->GetViewInfo());
    ResourceBarrier(dst->GetResource(), ResourceState::CopyDest, &dst->GetViewInfo());

    auto srcTex = dynamic_cast<VulkanTexture *>(src->GetResource());
    if (srcTex && srcTex->GetSampleCount() > 1)
    {
        VkImageResolve resolve;
        InitBlitData<1>(src, srcRect, resolve.srcSubresource, &resolve.srcOffset);
        InitBlitData<1>(dst, dstRect, resolve.dstSubresource, &resolve.dstOffset);
        const auto &viewInfo = src->GetViewInfo();
        resolve.extent.width = srcTex->GetWidth(viewInfo.mostDetailedMip);
        resolve.extent.height = srcTex->GetHeight(viewInfo.mostDetailedMip);
        resolve.extent.depth = 1;

        auto dstTex = dynamic_cast<VulkanTexture *>(dst->GetResource());
        vkCmdResolveImage(contextData->GetCommandBufferHandle(), srcTex->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTex->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &resolve);
    }
    else
    {
        VkImageBlit blit;
        InitBlitData<2>(src, srcRect, blit.srcSubresource, blit.srcOffsets);
        InitBlitData<2>(dst, dstRect, blit.dstSubresource, blit.dstOffsets);

        // Vulkan spec requires VK_FILTER_NEAREST if blit source is a depth and/or stencil format
        auto dstTex = dynamic_cast<VulkanTexture *>(dst->GetResource());
        VkFilter vkFilter = IsDepthStencilFormat(srcTex->GetResourceFormat()) ? VK_FILTER_NEAREST : ToVkFilter(filter);
        vkCmdBlitImage(contextData->GetCommandBufferHandle(), srcTex->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTex->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, vkFilter);
    }

    commandsPending = true;
}

void VulkanRenderContextImpl::ResolveResource(Texture *src, Texture *dst)
{
    Blit(src->GetSrv().get(), dst->GetRtv().get());
}

void VulkanRenderContextImpl::ResolveSubresource(Texture *src, int32 srcSub, Texture *dst, int32 dstSub)
{
    int32 srcArray = src->GetSubresourceArraySlice(srcSub);
    int32 srcMip = src->GetSubresourceMipLevel(srcSub);
    auto srcSrv = src->GetSrv(srcMip, 1, srcArray, 1);

    int32 dstArray = dst->GetSubresourceArraySlice(dstSub);
    int32 dstMip = dst->GetSubresourceMipLevel(dstSub);
    auto dstRtv = dst->GetRtv(dstMip, dstArray, 1);

    Blit(srcSrv.get(), dstRtv.get());
}

bool VulkanRenderContextImpl::ApplyGraphicsVars(GraphicsVars *vars, RootSignature *rootSignature)
{
    if (vars->Apply(renderContext, rootSignature) == false)
    {
        CT_LOG(Warning, CT_TEXT("Call ApplyGraphicsVars() failed."));
        return false;
    }
    return true;
}

void VulkanRenderContextImpl::SetPipelineState(GraphicsStateObject *gso)
{
    auto commandBuffer = contextData->GetCommandBufferHandle();
    auto handle = static_cast<VulkanGraphicsStateObject *>(gso)->GetHandle();
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, handle);
}

void VulkanRenderContextImpl::SetVao(const VertexArray *vao)
{
    auto commandBuffer = contextData->GetCommandBufferHandle();
    const auto &buffers = vao->GetVertexBuffers();
    for (int32 i = 0; i < buffers.Count(); ++i)
    {
        auto vkBuffer = static_cast<VulkanBuffer *>(buffers[i].get());
        VkDeviceSize offset = vkBuffer->GetOffset();
        VkBuffer handle = vkBuffer->GetHandle();
        vkCmdBindVertexBuffers(commandBuffer, i, 1, &handle, &offset);
        ResourceBarrier(vkBuffer, ResourceState::VertexBuffer, nullptr);
    }
    if (vao->GetIndexBuffer())
    {
        auto vkBuffer = static_cast<VulkanBuffer *>(vao->GetIndexBuffer().get());
        VkDeviceSize offset = vkBuffer->GetOffset();
        VkBuffer handle = vkBuffer->GetHandle();
        vkCmdBindIndexBuffer(commandBuffer, handle, offset, ToVkIndexType(vao->GetIndexBufferFormat()));
        ResourceBarrier(vkBuffer, ResourceState::IndexBuffer, nullptr);
    }
}

void VulkanRenderContextImpl::SetFbo(const FrameBuffer *fbo)
{
    for (int i = 0; i < fbo->GetColorTextureCount(); ++i)
    {
        const auto &texture = fbo->GetColorTexture(i);
        ResourceBarrier(texture.get(), ResourceState::RenderTarget, nullptr);
    }
    if (fbo->GetDepthStencilTexture())
    {
        ResourceBarrier(fbo->GetDepthStencilTexture().get(), ResourceState::DepthStencil, nullptr);
    }
}

void VulkanRenderContextImpl::SetViewports(const Array<Viewport> &viewports)
{
    auto commandBuffer = contextData->GetCommandBufferHandle();

    VkViewport vkViewports[VIEWPORT_MAX_NUM];
    for (int32 i = 0; i < viewports.Count(); ++i)
    {
        vkViewports[i].x = viewports[i].x;
        vkViewports[i].y = viewports[i].y;
        vkViewports[i].width = viewports[i].width;
        vkViewports[i].height = viewports[i].height;
        vkViewports[i].minDepth = viewports[i].minDepth;
        vkViewports[i].maxDepth = viewports[i].maxDepth;
    }
    vkCmdSetViewport(commandBuffer, 0, viewports.Count(), vkViewports);
}

void VulkanRenderContextImpl::SetScissors(const Array<Scissor> &scissors)
{
    auto commandBuffer = contextData->GetCommandBufferHandle();

    VkRect2D vkScissors[VIEWPORT_MAX_NUM];
    for (int32 i = 0; i < scissors.Count(); ++i)
    {
        vkScissors[i].offset.x = scissors[i].x;
        vkScissors[i].offset.y = scissors[i].y;
        vkScissors[i].extent.width = scissors[i].width;
        vkScissors[i].extent.height = scissors[i].height;
    }
    vkCmdSetScissor(commandBuffer, 0, scissors.Count(), vkScissors);
}

bool VulkanRenderContextImpl::PrepareForDraw(GraphicsState *state, GraphicsVars *vars)
{
    auto gso = state->GetGso(vars);

    if (bindFlags & GraphicsStateBind::Vars)
    {
        if (vars)
        {
            if (!ApplyGraphicsVars(vars, gso->GetDesc().rootSignature.get()))
                return false;
        }
    }

    if (bindFlags & GraphicsStateBind::PipelineState)
        SetPipelineState(gso.get());
    if (bindFlags & GraphicsStateBind::Vao)
        SetVao(state->GetVertexArray().get());
    if (bindFlags & GraphicsStateBind::Fbo)
        SetFbo(state->GetFrameBuffer().get());
    if (bindFlags & GraphicsStateBind::Viewports)
        SetViewports(state->GetViewports());
    if (bindFlags & GraphicsStateBind::Scissors)
        SetScissors(state->GetScissors());

    auto vkFbo = static_cast<VulkanFrameBuffer *>(state->GetFrameBuffer().get());
    VkRenderPassBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.renderPass = vkFbo->GetRenderPassHandle();
    beginInfo.framebuffer = vkFbo->GetHandle();
    beginInfo.renderArea.offset = { 0, 0 };
    beginInfo.renderArea.extent = { (uint32)vkFbo->GetWidth(), (uint32)vkFbo->GetHeight() };
    // Only needed if attachments use VK_ATTACHMENT_LOAD_OP_CLEAR
    beginInfo.clearValueCount = 0;
    beginInfo.pClearValues = nullptr;

    vkCmdBeginRenderPass(contextData->GetCommandBufferHandle(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

    return true;
}
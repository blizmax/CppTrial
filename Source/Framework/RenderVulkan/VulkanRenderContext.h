#pragma once

#include "RenderVulkan/VulkanComputeContext.h"
#include "RenderCore/RenderContext.h"

namespace RenderCore
{

class VulkanRenderContext : public VulkanComputeContext, public RenderContext
{
public:
    VulkanRenderContext(const SPtr<GpuQueue> &queue);
    virtual ~VulkanRenderContext();

    virtual void ClearFrameBuffer(const FrameBuffer *fbo, const Color &color, float depth, uint8 stencil, bool clearColor, bool clearDepthStencil) override;
    virtual void ClearRtv(const ResourceView *rtv, const Color &color) override;
    virtual void ClearDsv(const ResourceView *dsv, float depth, uint8 stencil, bool clearDepth, bool clearStencil) override;
    virtual void ClearTexture(Texture *texture, const Color &color) override;
    virtual void Draw(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 firstVertex) override;
    virtual void DrawInstanced(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance) override;
    virtual void DrawIndexed(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 firstIndex, int32 vertexOffset) override;
    virtual void DrawIndexedInstanced(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 instanceCount, uint32 firstIndex, int32 vertexOffset, uint32 firstInstance) override;

    virtual void Blit(ResourceView *src, ResourceView *dst) override;
    virtual void ResolveResource(Texture *src, Texture *dst) override;
    virtual void ResolveSubresource(Texture *src, uint32 srcSub, Texture *dst, uint32 dstSub) override;

private:
    bool PrepareForDraw(GraphicsState *state, GraphicsVars *vars);

};


}
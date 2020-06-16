#pragma once

#include "RenderCore/ComputeContext.h"
#include "RenderCore/GraphicsState.h"

namespace RenderCore
{
class RenderContext : virtual public ComputeContext
{
public:
    virtual ~RenderContext() = default;

    virtual void ClearFrameBuffer(const FrameBuffer *fbo, const Color &color, float depth, uint8 stencil, bool clearColor = true, bool clearDepthStencil = true) = 0;
    virtual void ClearRtv(const ResourceView *rtv, const Color &color) = 0;
    virtual void ClearDsv(const ResourceView *dsv, float depth, uint8 stencil, bool clearDepth = true, bool clearStencil = true) = 0;
    virtual void ClearTexture(Texture *texture, const Color &color) = 0;
    virtual void Draw(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 firstVertex) = 0;
    virtual void DrawInstanced(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance) = 0;
    virtual void DrawIndexed(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 firstIndex, int32 vertexOffset) = 0;
    virtual void DrawIndexedInstanced(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 instanceCount, uint32 firstIndex, int32 vertexOffset, uint32 firstInstance) = 0;
    //TODO DrawIndirect

    /** From srv to rtv, rect: [left, up, right, down]  */
    virtual void Blit(ResourceView *srcSrv, ResourceView *dstRtv, const UVector4 &srcRect = UVector4(UINT32_MAX), const UVector4 &dstRect = UVector4(UINT32_MAX), TextureFilter filter = TextureFilter::Linear) = 0;
    virtual void ResolveResource(Texture *src, Texture *dst) = 0;
    virtual void ResolveSubresource(Texture *src, uint32 srcSub, Texture *dst, uint32 dstSub) = 0;

    virtual void SetBindFlags(GraphicsStateBindFlags flags) = 0;
    virtual GraphicsStateBindFlags GetBindFlags() const = 0;

    static SPtr<RenderContext> Create(const SPtr<GpuQueue> &queue);
};
}
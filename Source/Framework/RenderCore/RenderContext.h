#pragma once

#include "RenderCore/ComputeContext.h"
#include "RenderCore/GraphicsState.h"

class RenderContext : virtual public ComputeContext
{
public:
    virtual ~RenderContext() = default;

    virtual void ClearFrameBuffer(const FrameBuffer *fbo, const Color &color, float depth, uint8 stencil, bool clearColor = true, bool clearDepthStencil = true) = 0;
    virtual void ClearRtv(const ResourceView *rtv, const Color &color) = 0;
    virtual void ClearDsv(const ResourceView *dsv, float depth, uint8 stencil, bool clearDepth = true, bool clearStencil = true) = 0;
    virtual void ClearTexture(Texture *texture, const Color &color) = 0;
    virtual void Draw(GraphicsState *state, GraphicsVars *vars, int32 vertexCount, int32 firstVertex) = 0;
    virtual void DrawInstanced(GraphicsState *state, GraphicsVars *vars, int32 vertexCount, int32 instanceCount, int32 firstVertex, int32 firstInstance) = 0;
    virtual void DrawIndexed(GraphicsState *state, GraphicsVars *vars, int32 indexCount, int32 firstIndex, int32 vertexOffset) = 0;
    virtual void DrawIndexedInstanced(GraphicsState *state, GraphicsVars *vars, int32 indexCount, int32 instanceCount, int32 firstIndex, int32 vertexOffset, int32 firstInstance) = 0;
    //TODO DrawIndirect

    /** From srv to rtv, rect: [left, up, right, down]  */
    virtual void Blit(ResourceView *srcSrv, ResourceView *dstRtv, const Vector4I &srcRect = Vector4I(-1), const Vector4I &dstRect = Vector4I(-1), TextureFilter filter = TextureFilter::Linear) = 0;
    virtual void ResolveResource(Texture *src, Texture *dst) = 0;
    virtual void ResolveSubresource(Texture *src, int32 srcSub, Texture *dst, int32 dstSub) = 0;

    virtual void SetBindFlags(GraphicsStateBindFlags flags) = 0;
    virtual GraphicsStateBindFlags GetBindFlags() const = 0;

    static SPtr<RenderContext> Create(const SPtr<GpuQueue> &queue);
};
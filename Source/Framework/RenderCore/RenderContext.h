#pragma once

#include "RenderCore/ComputeContext.h"
#include "RenderCore/GraphicsState.h"

namespace RenderCore
{
class RenderContext : public ComputeContext
{
public:
    virtual ~RenderContext() = default;

    virtual void ClearFrameBuffer(const FrameBuffer *fbo, const Color &color, float depth, uint8 stencil) = 0;
    virtual void ClearRtv(const ResourceView *rtv, const Color &color) = 0;
    virtual void ClearDsv(const ResourceView *dsv, float depth, uint8 stencil) = 0;
    virtual void ClearTexture(Texture *texture, const Color &color) = 0;
    virtual void Draw(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 vertexOffset) = 0;
    virtual void DrawInstanced(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 instanceCount, uint32 vertexOffset, uint32 instanceOffset) = 0;
    virtual void DrawIndexed(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 indexOffset, uint32 vertexOffset) = 0;
    virtual void DrawIndexInstanced(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 instanceCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset) = 0;

    virtual void Blit(ResourceView *src, ResourceView *dst) = 0;
    virtual void ResolveResource(Texture *src, Texture *dst) = 0;
    virtual void ResolveSubresource(Texture *src, uint32 srcSub, Texture *dst, uint32 dstSub) = 0;

    static SPtr<RenderContext> Create(const SPtr<GpuQueue> &queue);
};
}
#pragma once

#include "RenderCore/RenderContext.h"
#include "RenderVulkan/VulkanComputeContext.h"

class VulkanRenderContextImpl : public VulkanComputeContextImpl
{
public:
    VulkanRenderContextImpl(const SPtr<GpuQueue> &queue, RenderContext *ctx);
    ~VulkanRenderContextImpl();

    void ClearFrameBuffer(const FrameBuffer *fbo, const Color &color, float depth, uint8 stencil, bool clearColor, bool clearDepthStencil);
    void ClearRtv(const ResourceView *rtv, const Color &color);
    void ClearDsv(const ResourceView *dsv, float depth, uint8 stencil, bool clearDepth, bool clearStencil);
    void ClearTexture(Texture *texture, const Color &color);
    void Draw(GraphicsState *state, GraphicsVars *vars, int32 vertexCount, int32 firstVertex);
    void DrawInstanced(GraphicsState *state, GraphicsVars *vars, int32 vertexCount, int32 instanceCount, int32 firstVertex, int32 firstInstance);
    void DrawIndexed(GraphicsState *state, GraphicsVars *vars, int32 indexCount, int32 firstIndex, int32 vertexOffset);
    void DrawIndexedInstanced(GraphicsState *state, GraphicsVars *vars, int32 indexCount, int32 instanceCount, int32 firstIndex, int32 vertexOffset, int32 firstInstance);
    void DrawIndirect(GraphicsState *state, GraphicsVars *vars, int32 drawCount, const Buffer *argBuffer, uint64 argBufferOffset, const Buffer *countBuffer, uint64 countBufferOffset);
    void DrawIndexedIndirect(GraphicsState *state, GraphicsVars *vars, int32 drawCount, const Buffer *argBuffer, uint64 argBufferOffset, const Buffer *countBuffer, uint64 countBufferOffset);

    void Blit(ResourceView *srcSrv, ResourceView *dstRtv, const Vector4I &srcRect = Vector4I(-1), const Vector4I &dstRect = Vector4I(-1), TextureFilter filter = TextureFilter::Linear);
    void ResolveResource(Texture *src, Texture *dst);
    void ResolveSubresource(Texture *src, int32 srcSub, Texture *dst, int32 dstSub);

    void SetBindFlags(GraphicsStateBindFlags flags)
    {
        bindFlags = flags;
    }

    GraphicsStateBindFlags GetBindFlags() const
    {
        return bindFlags;
    }

protected:
    bool ApplyGraphicsVars(GraphicsVars *vars, RootSignature *rootSignature);
    void SetPipelineState(GraphicsStateObject *gso);
    void SetVertexArray(const VertexArray *vao);
    void SetFrameBuffer(const FrameBuffer *fbo);
    void SetViewports(const Array<Viewport> &viewports);
    void SetScissors(const Array<Scissor> &scissors);
    bool PrepareForDraw(GraphicsState *state, GraphicsVars *vars);

protected:
    RenderContext *renderContext;
    GraphicsStateBindFlags bindFlags = GraphicsStateBind::All;
};

class VulkanRenderContext : public RenderContext
{
public:
    VulkanRenderContext(const SPtr<GpuQueue> &queue)
        : impl(queue, this)
    {
    }

    virtual void Flush(bool wait) override
    {
        impl.Flush(wait);
    }

    virtual bool ResourceBarrier(const Resource *resource, ResourceState newState, const ResourceViewInfo *viewInfo) override
    {
        return impl.ResourceBarrier(resource, newState, viewInfo);
    }

    virtual void UavBarrier(const Resource *resource) override
    {
        impl.UavBarrier(resource);
    }

    virtual void CopyResource(const Resource *dst, const Resource *src) override
    {
        impl.CopyResource(dst, src);
    }

    virtual void CopyBufferRegion(const Buffer *dst, uint64 dstOffset, const Buffer *src, uint64 srcOffset, uint32 size) override
    {
        impl.CopyBufferRegion(dst, dstOffset, src, srcOffset, size);
    }

    virtual void CopySubresource(const Texture *dst, int32 dstSub, const Texture *src, int32 srcSub) override
    {
        impl.CopySubresource(dst, dstSub, src, srcSub);
    }

    virtual void CopySubresourceRegion(const Texture *dst, int32 dstSub, const Texture *src, int32 srcSub, const Vector3I &dstOffset, const Vector3I &srcOffset, const Vector3I &size) override
    {
        impl.CopySubresourceRegion(dst, dstSub, src, srcSub, dstOffset, srcOffset, size);
    }

    virtual void UpdateBuffer(const Buffer *buffer, const void *data, int32 offset, uint32 size) override
    {
        impl.UpdateBuffer(buffer, data, offset, size);
    }

    virtual void UpdateTexture(const Texture *texture, const void *data) override
    {
        impl.UpdateTexture(texture, data);
    }

    virtual void UpdateSubresource(const Texture *texture, int32 subresource, const void *data, const Vector3I &offset, const Vector3I &size) override
    {
        impl.UpdateSubresource(texture, subresource, data, offset, size);
    }

    virtual void UpdateSubresources(const Texture *texture, int32 firstSub, int32 subCount, const void *data) override
    {
        impl.UpdateSubresources(texture, firstSub, subCount, data);
    }

    virtual Array<uint8> ReadSubresource(const Texture *texture, int32 subresource) override
    {
        return impl.ReadSubresource(texture, subresource);
    }

    virtual SPtr<ReadTextureTask> ReadSubresourceAsync(const Texture *texture, int32 subresource) override
    {
        return impl.ReadSubresourceAsync(texture, subresource);
    }

    virtual void Dispatch(ComputeState *state, ComputeVars *vars, const Vector3I &size) override
    {
        impl.Dispatch(state, vars, size);
    }

    virtual void DispatchIndirect(ComputeState *state, ComputeVars *vars, const Buffer *argBuffer, uint64 argBufferOffset) override
    {
        impl.DispatchIndirect(state, vars, argBuffer, argBufferOffset);
    }

    virtual void ClearUav(const ResourceView *uav, const Vector4 &value) override
    {
        impl.ClearUav(uav, value);
    }

    virtual void ClearUav(const ResourceView *uav, const Vector4U &value) override
    {
        impl.ClearUav(uav, value);
    }

    virtual void ClearUavCounter(const Buffer *buffer, uint32 value) override
    {
        impl.ClearUavCounter(buffer, value);
    }

    virtual void ClearFrameBuffer(const FrameBuffer *fbo, const Color &color, float depth, uint8 stencil, bool clearColor, bool clearDepthStencil) override
    {
        impl.ClearFrameBuffer(fbo, color, depth, stencil, clearColor, clearDepthStencil);
    }

    virtual void ClearRtv(const ResourceView *rtv, const Color &color) override
    {
        impl.ClearRtv(rtv, color);
    }

    virtual void ClearDsv(const ResourceView *dsv, float depth, uint8 stencil, bool clearDepth, bool clearStencil) override
    {
        impl.ClearDsv(dsv, depth, stencil, clearDepth, clearStencil);
    }

    virtual void ClearTexture(Texture *texture, const Color &color) override
    {
        impl.ClearTexture(texture, color);
    }

    virtual void Draw(GraphicsState *state, GraphicsVars *vars, int32 vertexCount, int32 firstVertex) override
    {
        impl.Draw(state, vars, vertexCount, firstVertex);
    }

    virtual void DrawInstanced(GraphicsState *state, GraphicsVars *vars, int32 vertexCount, int32 instanceCount, int32 firstVertex, int32 firstInstance) override
    {
        impl.DrawInstanced(state, vars, vertexCount, instanceCount, firstVertex, firstInstance);
    }

    virtual void DrawIndexed(GraphicsState *state, GraphicsVars *vars, int32 indexCount, int32 firstIndex, int32 vertexOffset) override
    {
        impl.DrawIndexed(state, vars, indexCount, firstIndex, vertexOffset);
    }

    virtual void DrawIndexedInstanced(GraphicsState *state, GraphicsVars *vars, int32 indexCount, int32 instanceCount, int32 firstIndex, int32 vertexOffset, int32 firstInstance) override
    {
        impl.DrawIndexedInstanced(state, vars, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    virtual void DrawIndirect(GraphicsState *state, GraphicsVars *vars, int32 drawCount, const Buffer *argBuffer, uint64 argBufferOffset, const Buffer *countBuffer, uint64 countBufferOffset) override
    {
        impl.DrawIndirect(state, vars, drawCount, argBuffer, argBufferOffset, countBuffer, countBufferOffset);
    }

    virtual void DrawIndexedIndirect(GraphicsState *state, GraphicsVars *vars, int32 drawCount, const Buffer *argBuffer, uint64 argBufferOffset, const Buffer *countBuffer, uint64 countBufferOffset) override
    {
        impl.DrawIndexedIndirect(state, vars, drawCount, argBuffer, argBufferOffset, countBuffer, countBufferOffset);
    }

    virtual void Blit(ResourceView *srcSrv, ResourceView *dstRtv, const Vector4I &srcRect, const Vector4I &dstRect, TextureFilter filter) override
    {
        impl.Blit(srcSrv, dstRtv, srcRect, dstRect, filter);
    }

    virtual void ResolveResource(Texture *src, Texture *dst) override
    {
        impl.ResolveResource(src, dst);
    }

    virtual void ResolveSubresource(Texture *src, int32 srcSub, Texture *dst, int32 dstSub) override
    {
        impl.ResolveSubresource(src, srcSub, dst, dstSub);
    }

    virtual void SetBindFlags(GraphicsStateBindFlags flags) override
    {
        impl.SetBindFlags(flags);
    }

    virtual GraphicsStateBindFlags GetBindFlags() const override
    {
        return impl.GetBindFlags();
    }

    const SPtr<VulkanContextData> &GetContextData()
    {
        return impl.GetContextData();
    }

private:
    VulkanRenderContextImpl impl;
};
#pragma once

#include "RenderVulkan/VulkanComputeContext.h"
#include "RenderCore/RenderContext.h"

namespace RenderCore
{

class VulkanRenderContextImpl : public VulkanComputeContextImpl
{
public:
    VulkanRenderContextImpl(const SPtr<GpuQueue> &queue, RenderContext *ctx);
    ~VulkanRenderContextImpl();

    void ClearFrameBuffer(const FrameBuffer *fbo, const Color &color, float depth, uint8 stencil, bool clearColor, bool clearDepthStencil);
    void ClearRtv(const ResourceView *rtv, const Color &color);
    void ClearDsv(const ResourceView *dsv, float depth, uint8 stencil, bool clearDepth, bool clearStencil);
    void ClearTexture(Texture *texture, const Color &color);
    void Draw(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 firstVertex);
    void DrawInstanced(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance);
    void DrawIndexed(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 firstIndex, int32 vertexOffset);
    void DrawIndexedInstanced(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 instanceCount, uint32 firstIndex, int32 vertexOffset, uint32 firstInstance);

    void Blit(ResourceView *srcSrv, ResourceView *dstRtv, const UVector4 &srcRect = UVector4(UINT32_MAX), const UVector4 &dstRect = UVector4(UINT32_MAX), TextureFilter filter = TextureFilter::Linear);
    void ResolveResource(Texture *src, Texture *dst);
    void ResolveSubresource(Texture *src, uint32 srcSub, Texture *dst, uint32 dstSub);

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
    void SetVao(const VertexArray *vao);
    void SetFbo(const FrameBuffer *fbo);
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
    VulkanRenderContext(const SPtr<GpuQueue> &queue) : impl(queue, this)
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

    virtual void CopyBufferRegion(const Buffer *dst, uint32 dstOffset, const Buffer *src, uint32 srcOffset, uint32 size) override
    {
        impl.CopyBufferRegion(dst, dstOffset, src, srcOffset, size);
    }

    virtual void CopySubresource(const Texture *dst, uint32 dstSub, const Texture *src, uint32 srcSub) override
    {
        impl.CopySubresource(dst, dstSub, src, srcSub);
    }

    virtual void CopySubresourceRegion(const Texture *dst, uint32 dstSub, const Texture *src, uint32 srcSub, const IVector3 &dstOffset, const IVector3 &srcOffset, const UVector3 &size) override
    {
        impl.CopySubresourceRegion(dst, dstSub, src, srcSub, dstOffset, srcOffset, size);
    }

    virtual void UpdateBuffer(const Buffer *buffer, const void *data, uint32 offset, uint32 size) override
    {
        impl.UpdateBuffer(buffer, data, offset, size);
    }

    virtual void UpdateTexture(const Texture *texture, const void *data) override
    {
        impl.UpdateTexture(texture, data);
    }

    virtual void UpdateSubresource(const Texture *texture, uint32 subresource, const void *data, const IVector3 &offset, const UVector3 &size) override
    {
        impl.UpdateSubresource(texture, subresource, data, offset, size);
    }

    virtual void UpdateSubresources(const Texture *texture, uint32 firstSub, uint32 subCount, const void *data) override
    {
        impl.UpdateSubresources(texture, firstSub, subCount, data);
    }

    virtual Array<uint8> ReadSubresource(const Texture *texture, uint32 subresource) override
    {
        return impl.ReadSubresource(texture, subresource);
    }

    virtual SPtr<ReadTextureTask> ReadSubresourceAsync(const Texture *texture, uint32 subresource) override
    {
        return impl.ReadSubresourceAsync(texture, subresource);
    }

    virtual void Dispatch(ComputeState *state, ComputeVars *vars, const UVector3 &size) override
    {
        impl.Dispatch(state, vars, size);
    }

    virtual void DispatchIndirect(ComputeState *state, ComputeVars *vars, const Buffer *argBuffer, uint32 argBufferOffset) override
    {
        impl.DispatchIndirect(state, vars, argBuffer, argBufferOffset);
    }

    virtual void ClearUav(const ResourceView *uav, const Vector4 &value) override
    {
        impl.ClearUav(uav, value);
    }

    virtual void ClearUav(const ResourceView *uav, const UVector4 &value) override
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

    virtual void Draw(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 firstVertex) override
    {
        impl.Draw(state, vars, vertexCount, firstVertex);
    }

    virtual void DrawInstanced(GraphicsState *state, GraphicsVars *vars, uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance) override
    {
        impl.DrawInstanced(state, vars, vertexCount, instanceCount, firstVertex, firstInstance);
    }
    
    virtual void DrawIndexed(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 firstIndex, int32 vertexOffset) override
    {
        impl.DrawIndexed(state, vars, indexCount, firstIndex, vertexOffset);
    }

    virtual void DrawIndexedInstanced(GraphicsState *state, GraphicsVars *vars, uint32 indexCount, uint32 instanceCount, uint32 firstIndex, int32 vertexOffset, uint32 firstInstance) override
    {
        impl.DrawIndexedInstanced(state, vars, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    virtual void Blit(ResourceView *srcSrv, ResourceView *dstRtv, const UVector4 &srcRect, const UVector4 &dstRect, TextureFilter filter) override
    {
        impl.Blit(srcSrv, dstRtv, srcRect, dstRect, filter);
    }

    virtual void ResolveResource(Texture *src, Texture *dst) override
    {
        impl.ResolveResource(src, dst);
    }

    virtual void ResolveSubresource(Texture *src, uint32 srcSub, Texture *dst, uint32 dstSub) override
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


}
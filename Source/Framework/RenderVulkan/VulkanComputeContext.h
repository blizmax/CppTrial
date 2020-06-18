#pragma once

#include "RenderVulkan/VulkanCopyContext.h"
#include "RenderCore/ComputeContext.h"

class VulkanComputeContextImpl : public VulkanCopyContextImpl
{
public:
    VulkanComputeContextImpl(const SPtr<GpuQueue> &queue, ComputeContext *ctx);
    ~VulkanComputeContextImpl();

    void Dispatch(ComputeState *state, ComputeVars *vars, const Vector3U &size);
    void DispatchIndirect(ComputeState *state, ComputeVars *vars, const Buffer *argBuffer, uint32 argBufferOffset);
    void ClearUav(const ResourceView *uav, const Vector4 &value);
    void ClearUav(const ResourceView *uav, const Vector4U &value) ;
    void ClearUavCounter(const Buffer *buffer, uint32 value);

protected:
    void ClearColorImage(const ResourceView *view, float v0, float v1, float v2, float v3);
    void ClearColorImage(const ResourceView *view, uint32 v0, uint32 v1, uint32 v2, uint32 v3);
    bool PrepareForDispatch(ComputeState *state, ComputeVars *vars);

protected:
    ComputeContext *computeContext;
};

class VulkanComputeContext : public ComputeContext
{
public:
    VulkanComputeContext(const SPtr<GpuQueue> &queue) : impl(queue, this)
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

    virtual void CopyBufferRegion(const Buffer *dst, int32 dstOffset, const Buffer *src, int32 srcOffset, uint32 size) override
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

    virtual void Dispatch(ComputeState *state, ComputeVars *vars, const Vector3U &size) override
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

    virtual void ClearUav(const ResourceView *uav, const Vector4U &value) override
    {
        impl.ClearUav(uav, value);
    }

    virtual void ClearUavCounter(const Buffer *buffer, uint32 value) override
    {
        impl.ClearUavCounter(buffer, value);
    }

    const SPtr<VulkanContextData> &GetContextData()
    {
        return impl.GetContextData();
    }

private:
    VulkanComputeContextImpl impl;
};
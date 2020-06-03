#pragma once

#include "RenderVulkan/VulkanContextData.h"
#include "RenderCore/CopyContext.h"

namespace RenderCore
{
class VulkanCopyContextImpl
{
public:
    VulkanCopyContextImpl(const SPtr<GpuQueue> &queue, CopyContext *ctx);
    ~VulkanCopyContextImpl();

    virtual void Flush(bool wait);

    bool ResourceBarrier(const Resource *resource, ResourceState newState, const ResourceViewInfo *viewInfo);
    void UavBarrier(const Resource *resource);
    void CopyResource(const Resource *dst, const Resource *src);
    void CopyBufferRegion(const Buffer *dst, uint32 dstOffset, const Buffer *src, uint32 srcOffset, uint32 size);
    void CopySubresource(const Texture *dst, uint32 dstSub, const Texture *src, uint32 srcSub);
    void CopySubresourceRegion(const Texture *dst, uint32 dstSub, const Texture *src, uint32 srcSub, const IVector3 &dstOffset, const IVector3 &srcOffset, const UVector3 &size);
    void UpdateBuffer(const Buffer *buffer, const void *data, uint32 offset, uint32 size);
    void UpdateTexture(const Texture *texture, const void *data);
    void UpdateSubresource(const Texture *texture, uint32 subresource, const void* data, const IVector3 &offset, const UVector3 &size);
    void UpdateSubresources(const Texture *texture, uint32 firstSub, uint32 subCount, const void* data);
    Array<uint8> ReadSubresource(const Texture *texture, uint32 subresource);
    SPtr<ReadTextureTask> ReadSubresourceAsync(const Texture *texture, uint32 subresource);
    const SPtr<VulkanContextData> &GetContextData() {return contextData;}

protected:
    bool BufferBarrier(const Buffer *buffer, ResourceState newState);
    bool TextureBarrier(const Texture *texture, ResourceState newState);
    bool SubresouceBarriers(const Texture *texture, ResourceState newState, const ResourceViewInfo *viewInfo);

protected:
    SPtr<VulkanContextData> contextData;
    bool commandsPending = false;
    CopyContext *copyContext;
};

class VulkanCopyContext : public CopyContext
{
public:
    VulkanCopyContext(const SPtr<GpuQueue> &queue) : impl(queue, this)
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

    const SPtr<VulkanContextData> &GetContextData()
    {
        return impl.GetContextData();
    }

private:
    VulkanCopyContextImpl impl;
};
}
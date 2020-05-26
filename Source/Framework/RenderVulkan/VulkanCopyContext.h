#pragma once

#include "RenderVulkan/VulkanContextData.h"
#include "RenderCore/CopyContext.h"

namespace RenderCore
{
class VulkanCopyContext : public CopyContext
{
public:
    VulkanCopyContext(const SPtr<GpuQueue> &queue);
    virtual ~VulkanCopyContext();

    virtual void Flush(bool wait) override;

    virtual bool ResourceBarrier(const Resource *resource, ResourceState newState, const ResourceViewInfo *viewInfo) override;
    virtual void UavBarrier(const Resource *resource) override;
    virtual void CopyResource(const Resource *dst, const Resource *src) override;
    virtual void CopyBufferRegion(const Buffer *dst, uint32 dstOffset, const Buffer *src, uint32 srcOffset, uint32 size) override;
    virtual void CopySubresource(const Texture *dst, uint32 dstSub, const Texture *src, uint32 srcSub) override;
    virtual void CopySubresourceRegion(const Texture *dst, uint32 dstSub, const Texture *src, uint32 srcSub, const IVector3 &dstOffset, const IVector3 &srcOffset, const UVector3 &size) override;
    virtual void UpdateBuffer(const Buffer *buffer, const void *data, uint32 offset, uint32 size) override;
    virtual void UpdateTexture(const Texture *texture, const void *data) override;
    virtual void UpdateSubresource(const Texture *texture, uint32 subresource, const void* data, const IVector3 &offset, const UVector3 &size) override;
    virtual void UpdateSubresources(const Texture *texture, uint32 firstSub, uint32 subCount, const void* data) override;
    virtual Array<uint8> ReadSubresource(const Texture *texture, uint32 subresource) override;
    virtual SPtr<ReadTextureTask> ReadSubresourceAsync(const Texture *texture, uint32 subresource) override;

    const SPtr<VulkanContextData> &GetContextData()
    {
        return contextData;
    }

protected:
    bool BufferBarrier(const Buffer *buffer, ResourceState newState);
    bool TextureBarrier(const Texture *texture, ResourceState newState);
    bool SubresouceBarriers(const Texture *texture, ResourceState newState, const ResourceViewInfo *viewInfo);

protected:
    SPtr<VulkanContextData> contextData;
    bool commandsPending = false;
};
}
#pragma once

#include "RenderVulkan/.Package.h"
#include "RenderCore/CopyContext.h"
#include "RenderVulkan/VulkanContextData.h"

namespace RenderCore
{
class VulkanCopyContext : public CopyContext
{
public:
    VulkanCopyContext(const SPtr<GpuQueue> &queue);
    virtual ~VulkanCopyContext();

    virtual bool ResourceBarrier(const Resource *resource, ResourceState newState, const ResourceViewInfo *viewInfo) override;
    virtual void UavBarrier(const Resource *resource) override;
    virtual void CopyResource(const Resource *dst, const Resource *src) override;

    virtual void Flush(bool wait) override;

private:
    bool bufferBarrier(const Buffer *pBuffer, ResourceState newState);
    bool textureBarrier(const Texture *pTexture, ResourceState newState);

private:
    SPtr<VulkanContextData> contextData;
};
}
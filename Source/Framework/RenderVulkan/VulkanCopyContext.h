#pragma once

#include "RenderVulkan/.Package.h"
#include "RenderCore/CopyContext.h"

namespace RenderCore
{

class VulkanContextData
{

private:
    VkQueue queue;  
    VkCommandPool pool;
    VkCommandBuffer buffer;
};

class VulkanCopyContext : public CopyContext
{
public:
    VulkanCopyContext(void *handle);
    virtual ~VulkanCopyContext();

    virtual bool ResourceBarrier(const Resource *resource, ResourceState newState, const ResourceViewInfo *viewInfo = nullptr);
    virtual void UavBarrier(const Resource *resource);

    virtual void Flush(bool wait);

    static SPtr<RenderContext> Create();

protected:

};
}
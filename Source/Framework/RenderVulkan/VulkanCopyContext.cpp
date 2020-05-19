#include "RenderVulkan/VulkanCopyContext.h"

namespace RenderCore
{

SPtr<CopyContext> CopyContext::Create(const SPtr<GpuQueue> &queue)
{
    return Memory::MakeShared<VulkanCopyContext>(queue);
}

VulkanCopyContext::VulkanCopyContext(const SPtr<GpuQueue> &queue)
{
    contextData = VulkanContextData::Create(queue);
}

VulkanCopyContext::~VulkanCopyContext()
{
}

bool VulkanCopyContext::ResourceBarrier(const Resource *resource, ResourceState newState, const ResourceViewInfo *viewInfo)
{
    //TODO
}
}
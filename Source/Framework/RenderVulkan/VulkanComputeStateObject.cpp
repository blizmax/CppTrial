#include "RenderVulkan/VulkanComputeStateObject.h"
#include "RenderVulkan/VulkanDevice.h"

SPtr<ComputeStateObject> ComputeStateObject::Create(const ComputeStateObjectDesc &desc)
{
    return Memory::MakeShared<VulkanComputeStateObject>(desc);
}

VulkanComputeStateObject::VulkanComputeStateObject(const ComputeStateObjectDesc &desc)
    : ComputeStateObject(desc)
{
    //TODO
}

VulkanComputeStateObject::~VulkanComputeStateObject()
{
    //TODO
}
#include "RenderVulkan/VulkanDescriptorPool.h"
#include "RenderVulkan/VulkanDevice.h"

SPtr<DescriptorPool> DescriptorPool::Create(const DescriptorPoolDesc &desc, const SPtr<GpuFence> &fence)
{
    return Memory::MakeShared<VulkanDescriptorPool>(desc, fence);
}

VulkanDescriptorPool::VulkanDescriptorPool(const DescriptorPoolDesc &desc, const SPtr<GpuFence> &fence)
    : DescriptorPool(desc, fence)
{
    int32 totalCount = 0;
    Array<VkDescriptorPoolSize> poolSizes;

    for (int32 i = 0; i < (int32)DescriptorType::Count; ++i)
    {
        if(desc.descCount[i])
        {
            VkDescriptorPoolSize s = {};
            s.type = ToVkDescriptorType((DescriptorType)i);
            s.descriptorCount = desc.descCount[i];
            poolSizes.Add(s);
            totalCount += desc.descCount[i];
        }
    }

    VkDescriptorPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    info.maxSets = totalCount;
    info.poolSizeCount = poolSizes.Count();
    info.pPoolSizes = poolSizes.GetData();
    info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    if (vkCreateDescriptorPool(gVulkanDevice->GetLogicalDeviceHandle(), &info, gVulkanAlloc, &pool) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create descriptor pool failed.");
}

VulkanDescriptorPool::~VulkanDescriptorPool()
{
    if(pool != VK_NULL_HANDLE)
    {
        if(gVulkanDevice)
        {
            gVulkanDevice->Release([pool = pool]() {
                vkDestroyDescriptorPool(gVulkanDevice->GetLogicalDeviceHandle(), pool, gVulkanAlloc);
            });
        }
        pool = VK_NULL_HANDLE;
    }
}
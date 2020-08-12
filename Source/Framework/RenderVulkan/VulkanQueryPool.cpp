#include "RenderVulkan/VulkanQueryPool.h"
#include "RenderVulkan/VulkanDevice.h"

VulkanQueryPool::VulkanQueryPool(const QueryPoolDesc &desc)
    : QueryPool(desc)
{
    CT_CHECK(desc.count > 0);

    VkQueryPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
    info.queryCount = desc.count;
    info.queryType = ToVkQueryType(desc.queryType);
    info.pipelineStatistics = VK_QUERY_PIPELINE_STATISTIC_FLAG_BITS_MAX_ENUM;

    if (vkCreateQueryPool(gVulkanDevice->GetLogicalDeviceHandle(), &info, gVulkanAlloc, &pool) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create query pool failed.");
}

VulkanQueryPool::~VulkanQueryPool()
{
    if (pool != VK_NULL_HANDLE)
    {
        if (gVulkanDevice)
        {
            gVulkanDevice->Release([pool = pool]() {
                vkDestroyQueryPool(gVulkanDevice->GetLogicalDeviceHandle(), pool, gVulkanAlloc);
            });
        }
        pool = VK_NULL_HANDLE;
    }
}
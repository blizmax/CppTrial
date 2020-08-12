#pragma once

#include "RenderCore/QueryPool.h"
#include "RenderVulkan/VulkanMemory.h"

class VulkanQueryPool : public QueryPool
{
public:
    VulkanQueryPool(const QueryPoolDesc &desc);
    ~VulkanQueryPool();

    VkQueryPool GetHandle() const
    {
        return pool;
    }

private:
    VkQueryPool pool = VK_NULL_HANDLE;
};
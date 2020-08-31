#include "RenderVulkan/VulkanGpuTimer.h"
#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/VulkanQueryPool.h"
#include "RenderVulkan/VulkanRenderContext.h"

SPtr<GpuTimer> GpuTimer::Create()
{
    return Memory::MakeShared<VulkanGpuTimer>();
}

VulkanGpuTimer::VulkanGpuTimer()
{
    auto pool = std::static_pointer_cast<VulkanQueryPool>(gVulkanDevice->GetQueryPool(QueryType::Timestamp));

    startID = pool->Allocate();
    endID = pool->Allocate();
    CT_CHECK(endID == (startID + 1));
}

VulkanGpuTimer::~VulkanGpuTimer()
{
    auto &pool = gVulkanDevice->GetQueryPool(QueryType::Timestamp);

    pool->Release(startID);
    pool->Release(endID);
}

void VulkanGpuTimer::Start()
{
    if (running)
    {
        CT_LOG(Warning, CT_TEXT("GpuTimer::Start() failed, timer is running."));
        return;
    }

    running = true;

    auto pool = std::static_pointer_cast<VulkanQueryPool>(gVulkanDevice->GetQueryPool(QueryType::Timestamp));
    auto vkCtx = static_cast<VulkanRenderContext *>(gVulkanDevice->GetRenderContext());
    vkCmdResetQueryPool(vkCtx->GetContextData()->GetCommandBufferHandle(), pool->GetHandle(), (uint32)startID, 2);
    vkCmdWriteTimestamp(vkCtx->GetContextData()->GetCommandBufferHandle(), VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, pool->GetHandle(), (uint32)startID);
}

void VulkanGpuTimer::Stop()
{
    if (!running)
    {
        CT_LOG(Warning, CT_TEXT("GpuTimer::Stop() failed, timer is not running."));
        return;
    }

    running = false;
    unfetched = true;

    auto pool = std::static_pointer_cast<VulkanQueryPool>(gVulkanDevice->GetQueryPool(QueryType::Timestamp));
    auto vkCtx = static_cast<VulkanRenderContext *>(gVulkanDevice->GetRenderContext());
    vkCmdWriteTimestamp(vkCtx->GetContextData()->GetCommandBufferHandle(), VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, pool->GetHandle(), (uint32)endID);
}

float VulkanGpuTimer::GetElapsedMs()
{
    if (running)
    {
        CT_LOG(Warning, CT_TEXT("GpuTimer::GetElapsedMs() cannot be called before Stop(), return 0.0 instead."));
        return 0.0f;
    }

    if (unfetched)
    {
        unfetched = false;

        uint64 result[2];
        auto pool = std::static_pointer_cast<VulkanQueryPool>(gVulkanDevice->GetQueryPool(QueryType::Timestamp));
        if (vkGetQueryPoolResults(gVulkanDevice->GetLogicalDeviceHandle(), pool->GetHandle(), (uint32)startID, 2, sizeof(uint64) * 2, result, sizeof(result[0]), VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT) != VK_SUCCESS)
            CT_EXCEPTION(RenderCore, "Get timestamp result failed.");

        auto startTs = (float)result[0];
        auto endTs = (float)result[1];
        elapsedMs = (endTs - startTs) * gVulkanDevice->GetTimestampPeriod();
    }

    return elapsedMs;
}

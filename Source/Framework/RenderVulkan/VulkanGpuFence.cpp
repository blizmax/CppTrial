#include "RenderVulkan/VulkanGpuFence.h"
#include "RenderVulkan/VulkanGpuQueue.h"
#include "RenderVulkan/VulkanDevice.h"
#include "Core/Math.h"

namespace RenderCore
{

SPtr<GpuFence> GpuFence::Create()
{
    return Memory::MakeShared<VulkanGpuFence>();
}

VulkanGpuFence::~VulkanGpuFence()
{
}

uint64 VulkanGpuFence::GpuSignal(GpuQueue *queue)
{
    static const int32 waitThreshold = 20;
    static const int32 waitCount = 10;
    static const VkPipelineStageFlags waitStages[waitCount] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    cpuValue++;
    auto &fence = GetVulkanFence();
    auto &sem = GetVulkanSemaphore();
    waitSemaphores.Add(sem->GetHandle());

    VkSemaphore semHandle = sem->GetHandle();
    VkSubmitInfo submit = {};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &semHandle;

    if (waitSemaphores.Count() > waitThreshold)
    {
        submit.pWaitDstStageMask = waitStages;
        submit.waitSemaphoreCount = waitCount;
        submit.pWaitSemaphores = waitSemaphores.GetData();
    }

    if (vkQueueSubmit(static_cast<VulkanGpuQueue *>(queue)->GetHandle(), 1, &submit, fence->GetHandle()) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Submit failed.");

    if (waitSemaphores.Count() > waitThreshold)
    {
        waitSemaphores.RemoveAt(waitSemaphores.Count() - waitCount, waitCount);
    }

    return cpuValue - 1;
}

void VulkanGpuFence::SyncGpu(GpuQueue *queue)
{
    Array<VkPipelineStageFlags> waitStages;
    waitStages.Add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, waitSemaphores.Count());

    VkSubmitInfo submit = {};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.waitSemaphoreCount = waitSemaphores.Count();
    submit.pWaitSemaphores = waitSemaphores.GetData();
    submit.pWaitDstStageMask = waitStages.GetData();
    if (vkQueueSubmit(static_cast<VulkanGpuQueue *>(queue)->GetHandle(), 1, &submit, nullptr) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Submit failed.");

    waitSemaphores.Clear();
}

void VulkanGpuFence::SyncCpu()
{
    if (activeFences.IsEmpty())
        return;

    Array<VkFence> fences;
    for (auto &f : activeFences)
    {
        fences.Add(f->GetHandle());
    }

    vkWaitForFences(gVulkanDevice->GetLogicalDeviceHandle(), fences.Count(), fences.GetData(), true, UINT64_MAX);

    gpuValue += fences.Count();

    for (auto &f : activeFences)
    {
        freeFences.Add(f);
    }
    activeFences.Clear();

    ReleaseSemaphores();
}

uint64 VulkanGpuFence::GetGpuValue()
{
    while (!activeFences.IsEmpty())
    {
        auto &fence = activeFences.First();
        if (fence->IsSignaled())
        {
            freeFences.Add(fence);
            activeFences.RemoveFirst();
            gpuValue++;
        }
        else
        {
            break;
        }
    }

    ReleaseSemaphores();
    return gpuValue;
}

void VulkanGpuFence::ReleaseSemaphores()
{
    int32 semCount = activeSemaphores.Count();
    int32 fenceCount = activeFences.Count();
    CT_CHECK(fenceCount <= semCount);
    int32 waitCount = waitSemaphores.Count();
    CT_CHECK(waitCount <= semCount);

    int32 count = Math::Min(semCount - fenceCount, semCount - waitCount);
    while (count > 0)
    {
        auto &sem = activeSemaphores.First();
        freeSemaphores.Add(sem);
        activeSemaphores.RemoveFirst();
        count--;
    }
}

const SPtr<VulkanFence> &VulkanGpuFence::GetVulkanFence()
{
    if (!freeFences.IsEmpty())
    {
        auto &fence = freeFences.Last();
        fence->Reset();
        activeFences.Add(fence);
        freeFences.RemoveLast();
    }
    else
    {
        activeFences.Add(VulkanFence::Create(false));
    }
    return activeFences.Last();
}

const SPtr<VulkanSemaphore> &VulkanGpuFence::GetVulkanSemaphore()
{
    if (!freeSemaphores.IsEmpty())
    {
        auto &sem = freeSemaphores.Last();
        activeSemaphores.Add(sem);
        freeSemaphores.RemoveLast();
    }
    else
    {
        activeSemaphores.Add(VulkanSemaphore::Create());
    }
    return activeSemaphores.Last();
}

}
#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderVulkan/VulkanQueue.h"

namespace RenderCore
{

struct VulkanCommandPoolCreateParams
{
    uint32 familyIndex;
    SPtr<VulkanQueue> queue;
};

class VulkanCommandPool : public IVulkanResource
{
public:
    VulkanCommandPool(const VulkanCommandPoolCreateParams &params);

    virtual void Destroy() override;
    SPtr<VulkanCommandBuffer> GetIdleBuffer();

    VkCommandPool GetHandle() const
    {
        return pool;
    }

    SPtr<VulkanQueue> GetQueue() const
    {
        return queue;
    }

    static SPtr<VulkanCommandPool> Create(const VulkanCommandPoolCreateParams &params);

private:
    SPtr<VulkanCommandBuffer> CreateBuffer();

private:
    VkCommandPool pool = VK_NULL_HANDLE;
    uint32 familyIndex;
    SPtr<VulkanQueue> queue;
    Array<SPtr<VulkanCommandBuffer>> buffers;
};

struct VulkanCommandBufferCreateParams
{
    VkCommandPool pool;
    SPtr<VulkanQueue> queue;
    bool secondary = false;
};

class VulkanCommandBuffer
{
public:
    enum class State
    {
        Ready,
        Recording,
        RecordingRenderPass,
        Recorded,
        Submitted,
    };

    VulkanCommandBuffer(const VulkanCommandBufferCreateParams& params);

    void Begin();
    void End();
    void BeginRenderPass();
    void EndRenderPass();
    void BindRenderPipeline();
    void Reset();
    void SetViewport(float x, float y, float width, float height);
    void SetScissor(int32 x, int32 y, uint32 width, uint32 height);
    void Draw(uint32 vertexOffset, uint32 vertexCount, uint32 instanceCount);

    VkCommandBuffer GetHandle() const
    {
        return buffer;
    }

    SPtr<VulkanQueue> GetQueue() const
    {
        return queue;
    }

    bool IsReady() const
    {
        return state == State::Ready;
    }

    bool IsReadyForSubmit() const
    {
        return state == State::Recorded;
    }

    void SetSubmitted()
    {
        state = State::Submitted;
    }

private:
    VkCommandBuffer buffer = VK_NULL_HANDLE;
    State state = State::Ready;
    SPtr<VulkanQueue> queue;
};    

}
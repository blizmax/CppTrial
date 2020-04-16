#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{
class VulkanCommandPool : public IVulkanResource
{
public:
    VulkanCommandPool(uint32 familyIndex);

    virtual void Destroy() override;

    VkCommandPool GetHandle() const
    {
        return pool;
    }

private:
    SPtr<VulkanCommandBuffer> CreateBuffer();

private:
    VkCommandPool pool = VK_NULL_HANDLE;
    uint32 familyIndex;
    Array<SPtr<VulkanCommandBuffer>> buffers;
};

struct VulkanCommandBufferCreateParams
{
    VkCommandPool pool;
    bool secondary = false;
};

class VulkanCommandBuffer
{
public:
    VulkanCommandBuffer(const VulkanCommandBufferCreateParams& params);

    void Begin();
    void End();
    void BeginRenderPass();
    void EndRenderPass();
    void BindRenderPipeline();
    void Draw(uint32 vertexOffset, uint32 vertexCount, uint32 instanceCount);

private:
    VkCommandBuffer buffer = VK_NULL_HANDLE;
};    

}
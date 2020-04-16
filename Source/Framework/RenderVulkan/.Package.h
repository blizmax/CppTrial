#pragma once

#include "RenderCore/.Package.h"

#include <vulkan/vulkan.h>

namespace RenderCore
{
    extern VkAllocationCallbacks *gVulkanAlloc;

    class VulkanDevice;
    class VulkanQueue;
    class VulkanImage;
    class VulkanRenderPass;
    class VulkanFrameBuffer;
    class VulkanCommandBuffer;
    class VulkanRenderPipeline;
    class VulkanSemaphore;
    class VulkanShader;
    class VulkanSwapChain;

    VkPolygonMode ToVkPolygonMode(PolygonMode mode);
    VkCullModeFlagBits ToVkCullMode(CullMode mode);
    VkBlendFactor ToVkBlendFactor(BlendFactor factor);
    VkBlendOp ToVkBlendOperation(BlendOperation operation);
    VkStencilOp ToVkStencilOperation(StencilOperation operation);
    VkCompareOp ToVkCompareOperation(CompareOperation operation);
    VkSampleCountFlagBits ToVkSampleCount(int32 sample);
}
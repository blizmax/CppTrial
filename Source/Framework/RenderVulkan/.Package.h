#pragma once

#include "RenderCore/.Package.h"

#include <vulkan/vulkan.h>

namespace RenderCore
{
    constexpr int32 VULKAN_FRAME_NUM = 2;

    class VulkanContext;

    extern VkAllocationCallbacks *gVulkanAlloc;
    extern VulkanContext *gVulkanContext;

    class VulkanDevice;
    class VulkanQueue;
    class VulkanImage;
    class VulkanRenderPass;
    class VulkanFrameBuffer;
    class VulkanCommandBuffer;
    class VulkanCommandPool;
    class VulkanRenderPipeline;
    class VulkanRenderPipelineState;
    class VulkanSemaphore;
    class VulkanFence;
    class VulkanShader;
    class VulkanShaderCompiler;
    class VulkanSwapChain;
    class VulkanVertexLayout;
    class VulkanVertexBuffer;
    class VulkanIndexBuffer;
    class VulkanUniformBuffer;

    VkImageLayout ToVkImageLayout(ResourceState state);
    VkShaderStageFlags ToVkShaderVisibility(ShaderVisibilityFlags visibility);
    VkDescriptorType ToVkDescriptorType(DescriptorType descType);
    VkMemoryPropertyFlags ToVkMemoryProperty(MemoryUsage usage);
    VkBufferUsageFlags ToVkBufferUsage(ResourceBindFlags bind);
    VkImageType ToVkImageType(ResourceType resourceType);
    VkImageUsageFlags ToVkImageUsage(ResourceBindFlags bind);

    VkPolygonMode ToVkPolygonMode(PolygonMode mode);
    VkCullModeFlagBits ToVkCullMode(CullMode mode);
    VkBlendFactor ToVkBlendFactor(BlendFactor factor);
    VkBlendOp ToVkBlendOperation(BlendOperation operation);
    VkStencilOp ToVkStencilOperation(StencilOperation operation);
    VkCompareOp ToVkCompareOperation(CompareOperation operation);
    VkSampleCountFlagBits ToVkSampleCount(int32 sample);
    VkSamplerAddressMode ToVkAddressMode(TextureWrap wrap);

    VkFormat ToVkResourceFormat(ResourceFormat format);
}
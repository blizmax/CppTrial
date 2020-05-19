#pragma once

#include "RenderCore/.Package.h"

#include <vulkan/vulkan.h>

namespace RenderCore
{
    constexpr int32 VULKAN_FRAME_NUM = 2;

    class VulkanDevice;
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
    class VulkanVertexLayout;

    extern VkAllocationCallbacks *gVulkanAlloc;
    extern VkInstance gVulkanInstance;
    extern VulkanDevice *gVulkanDevice;

    VkImageLayout ToVkImageLayout(ResourceState state);
    VkShaderStageFlags ToVkShaderVisibility(ShaderVisibilityFlags visibility);
    VkDescriptorType ToVkDescriptorType(DescriptorType descType);
    VkMemoryPropertyFlags ToVkMemoryProperty(MemoryUsage usage);
    VkBufferUsageFlags ToVkBufferUsage(ResourceBindFlags bind);
    VkImageType ToVkImageType(ResourceType resourceType);
    VkImageUsageFlags ToVkImageUsage(ResourceBindFlags bind);
    VkImageViewType ToVkImageViewType(ResourceType resourceType, bool array);
    VkImageAspectFlags ToVkImageAspect(ResourceFormat format, bool ignoreStencil = false);

    VkPrimitiveTopology ToVkTopology(Topology topology);
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
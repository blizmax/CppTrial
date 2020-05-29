#pragma once

#include "RenderCore/.Package.h"

#include <vulkan/vulkan.h>

namespace RenderCore
{
    constexpr int32 VULKAN_FRAME_NUM = 2;

    class VulkanDevice;
    class VulkanShaderCompiler;
    class VulkanVertexLayout;

    extern VkAllocationCallbacks *gVulkanAlloc;
    extern VkInstance gVulkanInstance;
    extern VulkanDevice *gVulkanDevice;
    extern VulkanShaderCompiler *gVulkanShaderCompiler;

    VkImageLayout ToVkImageLayout(ResourceState state);
    VkAccessFlags ToVkAccess(ResourceState state);
    VkPipelineStageFlags ToVkPipelineStage(ResourceState state, bool src);
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
    VkFilter ToVkFilter(TextureFilter filter);
    VkSamplerMipmapMode ToVkMipFilter(TextureFilter filter);
    VkBorderColor ToVkBorderColor(const Color &color);

    VkIndexType ToVkIndexType(ResourceFormat format);
    VkFormat ToVkResourceFormat(ResourceFormat format);
}
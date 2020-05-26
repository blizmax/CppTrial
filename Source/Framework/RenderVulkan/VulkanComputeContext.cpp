#include "RenderVulkan/VulkanComputeContext.h"
#include "RenderVulkan/VulkanBuffer.h"
#include "RenderVulkan/VulkanTexture.h"

namespace RenderCore
{

SPtr<ComputeContext> ComputeContext::Create(const SPtr<GpuQueue> &queue)
{
    return Memory::MakeShared<VulkanComputeContext>(queue);
}

VulkanComputeContext::VulkanComputeContext(const SPtr<GpuQueue> &queue)
    : VulkanCopyContext(queue)
{

}

VulkanComputeContext::~VulkanComputeContext()
{
}

void VulkanComputeContext::Dispatch(ComputeState *state, ComputeVars *vars, uint32 sizeX, uint32 sizeY, uint32 sizeZ)
{
    if(PrepareForDispatch(state, vars) == false)
        return;
    
    vkCmdDispatch(contextData->GetCommandBufferHandle(), sizeX, sizeY, sizeZ);
}

void VulkanComputeContext::DispatchIndirect(ComputeState *state, ComputeVars *vars, const Buffer *argBuffer, uint32 argBufferOffset)
{
    if(PrepareForDispatch(state, vars) == false)
        return;

    VulkanCopyContext::ResourceBarrier(argBuffer, ResourceState::IndirectArg, nullptr);
    auto vkBuffer = static_cast<const VulkanBuffer *>(argBuffer);
    vkCmdDispatchIndirect(contextData->GetCommandBufferHandle(), vkBuffer->GetHandle(), argBuffer->GetOffset() + argBufferOffset);
}

void VulkanComputeContext::ClearUav(const ResourceView *uav, const Vector4 &value)
{
    auto vkTexture = dynamic_cast<const VulkanTexture *>(uav->GetResource());
    CT_CHECK(vkTexture != nullptr);

    VulkanCopyContext::ResourceBarrier(vkTexture, ResourceState::CopyDest, nullptr);
    VkClearColorValue colVal = {};
    colVal.float32[0] = value.x;
    colVal.float32[1] = value.y;
    colVal.float32[2] = value.z;
    colVal.float32[3] = value.w;

    VkImageSubresourceRange range = {};
    const auto& viewInfo = uav->GetViewInfo();
    range.baseArrayLayer = viewInfo.firstArraySlice;
    range.baseMipLevel = viewInfo.mostDetailedMip;
    range.layerCount = viewInfo.arrayLayers;
    range.levelCount = viewInfo.mipLevels;
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    vkCmdClearColorImage(contextData->GetCommandBufferHandle(), vkTexture->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &colVal, 1, &range);

    commandsPending = true;
}

void VulkanComputeContext::ClearUav(const ResourceView *uav, const UVector4 &value)
{
    auto vkBuffer = dynamic_cast<const VulkanBuffer *>(uav->GetResource());
    if(vkBuffer)
    {
        vkCmdFillBuffer(contextData->GetCommandBufferHandle(), vkBuffer->GetHandle(), vkBuffer->GetOffset(), vkBuffer->GetSize(), value.x);
    }
    else
    {
        auto vkTexture = dynamic_cast<const VulkanTexture *>(uav->GetResource());
        CT_CHECK(vkTexture != nullptr);

        VulkanCopyContext::ResourceBarrier(vkTexture, ResourceState::CopyDest, nullptr);
        VkClearColorValue colVal = {};
        colVal.uint32[0] = value.x;
        colVal.uint32[1] = value.y;
        colVal.uint32[2] = value.z;
        colVal.uint32[3] = value.w;

        VkImageSubresourceRange range = {};
        const auto& viewInfo = uav->GetViewInfo();
        range.baseArrayLayer = viewInfo.firstArraySlice;
        range.baseMipLevel = viewInfo.mostDetailedMip;
        range.layerCount = viewInfo.arrayLayers;
        range.levelCount = viewInfo.mipLevels;
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        vkCmdClearColorImage(contextData->GetCommandBufferHandle(), vkTexture->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &colVal, 1, &range);
    }

    commandsPending = true;
}

void VulkanComputeContext::ClearUavCounter(const Buffer *buffer, uint32 value)
{
    auto uavCounter = buffer->GetUavCounter();
    if(uavCounter)
    {
        UVector4 uvec{value, 0, 0, 0};
        ClearUav(uavCounter->GetUav().get(), uvec);
    }
}

bool VulkanComputeContext::PrepareForDispatch(ComputeState *state, ComputeVars *vars)
{
    //TODO
    return true;
}

}
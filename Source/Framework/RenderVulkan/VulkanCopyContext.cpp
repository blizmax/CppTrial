#include "RenderVulkan/VulkanCopyContext.h"
#include "RenderVulkan/VulkanBuffer.h"
#include "RenderVulkan/VulkanTexture.h"

static uint32 GetMipLevelPackedDataSize(const Texture *texture, int32 w, int32 h, int32 d, ResourceFormat format)
{
    uint32 perW = GetResourceFormatWidthCompressionRatio(format);
    uint32 bw = CT_ALIGN(w, perW) / perW;

    uint32 perH = GetResourceFormatHeightCompressionRatio(format);
    uint32 bh = CT_ALIGN(h, perH) / perH;

    uint32 size = bh * bw * d * GetResourceFormatBytes(format);
    return size;
}

SPtr<ReadTextureTask> ReadTextureTask::Create(CopyContext *ctx, const Texture *texture, int32 subresource)
{
    VkBufferImageCopy vkCopy;
    int32 mipLevel = texture->GetSubresourceMipLevel(subresource);
    int32 w = texture->GetWidth(mipLevel);
    int32 h = texture->GetHeight(mipLevel);
    int32 d = texture->GetDepth(mipLevel);
    uint32 dataSize = GetMipLevelPackedDataSize(texture, w, h, d, texture->GetResourceFormat());

    VkBufferImageCopy copy = {};
    copy.bufferRowLength = 0;
    copy.bufferImageHeight = 0;
    copy.imageSubresource.aspectMask = ToVkImageAspect(texture->GetResourceFormat());
    copy.imageSubresource.baseArrayLayer = texture->GetSubresourceArraySlice(subresource);
    copy.imageSubresource.layerCount = 1;
    copy.imageSubresource.mipLevel = mipLevel;
    copy.imageOffset = { 0, 0, 0 };
    copy.imageExtent = { (uint32)w, (uint32)h, (uint32)d };
    auto downloadBuffer = Buffer::Create(dataSize, ResourceBind::None, BufferCpuAccess::Read, nullptr);
    copy.bufferOffset = downloadBuffer->GetGpuAddressOffset();

    auto vkCtx = static_cast<VulkanCopyContext *>(ctx);
    ctx->ResourceBarrier(texture, ResourceState::CopySource);
    ctx->ResourceBarrier(downloadBuffer.get(), ResourceState::CopyDest);

    auto srcHandle = static_cast<const VulkanTexture *>(texture)->GetHandle();
    auto dstHandle = static_cast<VulkanBuffer *>(downloadBuffer.get())->GetHandle();
    vkCmdCopyImageToBuffer(vkCtx->GetContextData()->GetCommandBufferHandle(), srcHandle, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstHandle, 1, &copy);

    auto task = Memory::MakeShared<ReadTextureTask>();
    task->buffer = downloadBuffer;
    task->size = dataSize;
    task->fence = GpuFence::Create();
    ctx->Flush(false);
    task->fence->GpuSignal(vkCtx->GetContextData()->GetQueue());

    return task;
}

SPtr<CopyContext> CopyContext::Create(const SPtr<GpuQueue> &queue)
{
    return Memory::MakeShared<VulkanCopyContext>(queue);
}

VulkanCopyContextImpl::VulkanCopyContextImpl(const SPtr<GpuQueue> &queue, CopyContext *ctx)
    : copyContext(ctx)
{
    contextData = VulkanContextData::Create(queue);
}

VulkanCopyContextImpl::~VulkanCopyContextImpl()
{
}

void VulkanCopyContextImpl::Flush(bool wait)
{
    if (commandsPending)
    {
        contextData->Flush();
        commandsPending = false;
    }
    else
    {
        contextData->GetFence()->GpuSignal(contextData->GetQueue());
    }

    if (wait)
    {
        contextData->GetFence()->SyncCpu();
    }
}

bool VulkanCopyContextImpl::ResourceBarrier(const Resource *resource, ResourceState newState, const ResourceViewInfo *viewInfo)
{
    const Texture *texture = dynamic_cast<const Texture *>(resource);
    if (texture)
    {
        bool global = texture->IsStateGlobal();
        if (!global && viewInfo)
        {
            if (viewInfo->firstArraySlice == 0 && viewInfo->mostDetailedMip == 0 && viewInfo->arrayLayers == texture->GetArrayLayers() && viewInfo->mipLevels == texture->GetMipLevels())
            {
                global = true;
            }
        }
        if (global)
        {
            return TextureBarrier(texture, newState);
        }
        else
        {
            return SubresouceBarriers(texture, newState, viewInfo);
        }
    }
    else
    {
        const Buffer *buffer = dynamic_cast<const Buffer *>(resource);
        return BufferBarrier(buffer, newState);
    }
}

void VulkanCopyContextImpl::UavBarrier(const Resource *resource)
{
    //CT_EXCEPTION(RenderCore, "Unsupported in vulkan.");
}

void VulkanCopyContextImpl::CopyResource(const Resource *dst, const Resource *src)
{
    const Buffer *dstBuffer = dynamic_cast<const Buffer *>(dst);
    if (dstBuffer)
    {
        const Buffer *srcBuffer = dynamic_cast<const Buffer *>(src);
        CT_CHECK(srcBuffer);
        CT_CHECK(srcBuffer->GetSize() == dstBuffer->GetSize());
        CopyBufferRegion(dstBuffer, 0, srcBuffer, 0, srcBuffer->GetSize());
    }
    else
    {
        const Texture *srcTex = dynamic_cast<const Texture *>(src);
        const Texture *dstTex = dynamic_cast<const Texture *>(dst);
        CT_CHECK(srcTex && dstTex);
        CT_CHECK(srcTex->GetArrayLayers() == dstTex->GetArrayLayers() && srcTex->GetMipLevels() == dstTex->GetMipLevels());

        VkImageAspectFlags srcAspect = ToVkImageAspect(srcTex->GetResourceFormat());
        VkImageAspectFlags dstAspect = ToVkImageAspect(dstTex->GetResourceFormat());
        int32 layerCount = srcTex->GetArrayLayers();
        Array<VkImageCopy> copies;
        for (int32 i = 0; i < srcTex->GetMipLevels(); ++i)
        {
            VkImageCopy copy = {};
            copy.srcSubresource.aspectMask = srcAspect;
            copy.srcSubresource.baseArrayLayer = 0;
            copy.srcSubresource.layerCount = layerCount;
            copy.srcSubresource.mipLevel = i;

            copy.dstSubresource = copy.srcSubresource;
            copy.dstSubresource.aspectMask = dstAspect;
            copy.extent.width = srcTex->GetWidth(i);
            copy.extent.height = srcTex->GetHeight(i);
            copy.extent.depth = srcTex->GetDepth(i);

            copies.Add(copy);
        }

        ResourceBarrier(dst, ResourceState::CopyDest, nullptr);
        ResourceBarrier(src, ResourceState::CopySource, nullptr);
        auto srcHandle = static_cast<const VulkanTexture *>(src)->GetHandle();
        auto dstHandle = static_cast<const VulkanTexture *>(dst)->GetHandle();
        vkCmdCopyImage(contextData->GetCommandBufferHandle(),
                       srcHandle, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstHandle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, copies.Count(), copies.GetData());

        commandsPending = true;
    }
}

void VulkanCopyContextImpl::CopyBufferRegion(const Buffer *dst, uint64 dstOffset, const Buffer *src, uint64 srcOffset, uint32 size)
{
    ResourceBarrier(dst, ResourceState::CopyDest, nullptr);
    ResourceBarrier(src, ResourceState::CopySource, nullptr);

    VkBufferCopy copy = {};
    copy.srcOffset = src->GetGpuAddressOffset() + srcOffset;
    copy.dstOffset = dst->GetGpuAddressOffset() + dstOffset;
    copy.size = size;

    auto srcHandle = static_cast<const VulkanBuffer *>(src)->GetHandle();
    auto dstHandle = static_cast<const VulkanBuffer *>(dst)->GetHandle();
    vkCmdCopyBuffer(contextData->GetCommandBufferHandle(), srcHandle, dstHandle, 1, &copy);

    commandsPending = true;
}

void VulkanCopyContextImpl::CopySubresource(const Texture *dst, int32 dstSub, const Texture *src, int32 srcSub)
{
    ResourceBarrier(dst, ResourceState::CopyDest, nullptr);
    ResourceBarrier(src, ResourceState::CopySource, nullptr);

    VkImageAspectFlags srcAspect = ToVkImageAspect(src->GetResourceFormat());
    VkImageAspectFlags dstAspect = ToVkImageAspect(dst->GetResourceFormat());
    VkImageCopy copy = {};
    copy.srcSubresource.aspectMask = srcAspect;
    copy.srcSubresource.baseArrayLayer = src->GetSubresourceArraySlice(srcSub);
    copy.srcSubresource.layerCount = 1;
    copy.srcSubresource.mipLevel = src->GetSubresourceMipLevel(srcSub);
    copy.dstSubresource.aspectMask = dstAspect;
    copy.dstSubresource.baseArrayLayer = dst->GetSubresourceArraySlice(dstSub);
    copy.dstSubresource.layerCount = 1;
    copy.dstSubresource.mipLevel = dst->GetSubresourceMipLevel(dstSub);
    int32 mipLevel = copy.dstSubresource.mipLevel;
    copy.extent.width = dst->GetWidth(mipLevel);
    copy.extent.height = dst->GetHeight(mipLevel);
    copy.extent.depth = dst->GetDepth(mipLevel);

    auto srcHandle = static_cast<const VulkanTexture *>(src)->GetHandle();
    auto dstHandle = static_cast<const VulkanTexture *>(dst)->GetHandle();
    vkCmdCopyImage(contextData->GetCommandBufferHandle(),
                   srcHandle, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstHandle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

    commandsPending = true;
}

void VulkanCopyContextImpl::CopySubresourceRegion(const Texture *dst, int32 dstSub, const Texture *src, int32 srcSub, const Vector3I &dstOffset, const Vector3I &srcOffset, const Vector3I &size)
{
    ResourceBarrier(dst, ResourceState::CopyDest, nullptr);
    ResourceBarrier(src, ResourceState::CopySource, nullptr);

    VkImageAspectFlags srcAspect = ToVkImageAspect(src->GetResourceFormat());
    VkImageAspectFlags dstAspect = ToVkImageAspect(dst->GetResourceFormat());
    VkImageCopy copy = {};
    copy.srcSubresource.layerCount = 1;
    copy.srcSubresource.baseArrayLayer = src->GetSubresourceArraySlice(srcSub);
    copy.srcSubresource.mipLevel = src->GetSubresourceMipLevel(srcSub);
    copy.srcSubresource.aspectMask = srcAspect;
    copy.dstSubresource.layerCount = 1;
    copy.dstSubresource.baseArrayLayer = dst->GetSubresourceArraySlice(dstSub);
    copy.dstSubresource.mipLevel = dst->GetSubresourceMipLevel(dstSub);
    copy.dstSubresource.aspectMask = dstAspect;

    int32 mipLevel = copy.srcSubresource.mipLevel;
    copy.dstOffset = { dstOffset.x, dstOffset.y, dstOffset.z };
    copy.srcOffset = { srcOffset.x, srcOffset.y, srcOffset.z };
    copy.extent.width = (size.x == -1) ? src->GetWidth(mipLevel) - srcOffset.x : size.x;
    copy.extent.height = (size.y == -1) ? src->GetHeight(mipLevel) - srcOffset.y : size.y;
    copy.extent.depth = (size.z == -1) ? src->GetDepth(mipLevel) - srcOffset.z : size.z;
    CT_CHECK(copy.extent.width >= 0 && copy.extent.height >= 0 && copy.extent.depth >= 0);

    auto srcHandle = static_cast<const VulkanTexture *>(src)->GetHandle();
    auto dstHandle = static_cast<const VulkanTexture *>(dst)->GetHandle();
    vkCmdCopyImage(contextData->GetCommandBufferHandle(),
                   srcHandle, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstHandle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

    commandsPending = true;
}

void VulkanCopyContextImpl::UpdateBuffer(const Buffer *buffer, const void *data, int32 offset, uint32 size)
{
    uint32 bufferSize = buffer->GetSize();
    CT_CHECK(offset >= 0 && offset < bufferSize);

    if (offset + size > bufferSize)
        size = bufferSize - offset;
    if (size == 0)
        return;

    auto uploadBuffer = Buffer::Create(size, ResourceBind::None, BufferCpuAccess::Write, data);
    CopyBufferRegion(buffer, offset, uploadBuffer.get(), 0, size);
}

void VulkanCopyContextImpl::UpdateTexture(const Texture *texture, const void *data)
{
    int32 subCount = texture->GetArrayLayers() * texture->GetMipLevels();
    if (texture->GetResourceType() == ResourceType::TextureCube)
    {
        subCount *= 6;
    }
    UpdateSubresources(texture, 0, subCount, data);
}

void VulkanCopyContextImpl::UpdateSubresource(const Texture *texture, int32 subresource, const void *data, const Vector3I &offset, const Vector3I &size)
{
    CT_CHECK(!IsDepthStencilFormat(texture->GetResourceFormat()));

    int32 mipLevel = texture->GetSubresourceMipLevel(subresource);
    int32 w = size.x;
    int32 h = size.y;
    int32 d = size.z;
    uint32 dataSize = GetMipLevelPackedDataSize(texture, w, h, d, texture->GetResourceFormat());

    VkBufferImageCopy copy = {};
    copy.bufferRowLength = 0;
    copy.bufferImageHeight = 0;
    copy.imageSubresource.aspectMask = ToVkImageAspect(texture->GetResourceFormat());
    copy.imageSubresource.baseArrayLayer = texture->GetSubresourceArraySlice(subresource);
    copy.imageSubresource.layerCount = 1;
    copy.imageSubresource.mipLevel = mipLevel;
    copy.imageOffset = { offset.x, offset.y, offset.z };

    copy.imageExtent.width = (size.x == -1) ? texture->GetWidth(mipLevel) - offset.x : size.x;
    copy.imageExtent.height = (size.y == -1) ? texture->GetHeight(mipLevel) - offset.y : size.y;
    copy.imageExtent.depth = (size.z == -1) ? texture->GetDepth(mipLevel) - offset.z : size.z;
    CT_CHECK(copy.imageExtent.width >= 0 && copy.imageExtent.height >= 0 && copy.imageExtent.depth >= 0);


    auto uploadBuffer = Buffer::Create(dataSize, ResourceBind::None, BufferCpuAccess::Write, data);
    copy.bufferOffset = uploadBuffer->GetGpuAddressOffset();

    ResourceBarrier(texture, ResourceState::CopyDest, nullptr);
    ResourceBarrier(uploadBuffer.get(), ResourceState::CopySource, nullptr);

    auto srcHandle = static_cast<VulkanBuffer *>(uploadBuffer.get())->GetHandle();
    auto dstHandle = static_cast<const VulkanTexture *>(texture)->GetHandle();
    vkCmdCopyBufferToImage(contextData->GetCommandBufferHandle(), srcHandle, dstHandle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

    commandsPending = true;
}

void VulkanCopyContextImpl::UpdateSubresources(const Texture *texture, int32 firstSub, int32 subCount, const void *data)
{
    CT_CHECK(!IsDepthStencilFormat(texture->GetResourceFormat()));

    const uint8 *dataSrc = static_cast<const uint8 *>(data);
    for (int32 i = 0; i < subCount; ++i)
    {
        int32 subresource = i + firstSub;
        int32 mipLevel = texture->GetSubresourceMipLevel(subresource);
        int32 w = texture->GetWidth(mipLevel);
        int32 h = texture->GetHeight(mipLevel);
        int32 d = texture->GetDepth(mipLevel);
        uint32 dataSize = GetMipLevelPackedDataSize(texture, w, h, d, texture->GetResourceFormat());

        VkBufferImageCopy copy = {};
        copy.bufferRowLength = 0;
        copy.bufferImageHeight = 0;
        copy.imageSubresource.aspectMask = ToVkImageAspect(texture->GetResourceFormat());
        copy.imageSubresource.baseArrayLayer = texture->GetSubresourceArraySlice(subresource);
        copy.imageSubresource.layerCount = 1;
        copy.imageSubresource.mipLevel = mipLevel;
        copy.imageOffset = { 0, 0, 0 };
        copy.imageExtent.width = w;
        copy.imageExtent.height = h;
        copy.imageExtent.depth = d;
        auto uploadBuffer = Buffer::Create(dataSize, ResourceBind::None, BufferCpuAccess::Write, dataSrc);
        copy.bufferOffset = uploadBuffer->GetGpuAddressOffset();

        ResourceBarrier(texture, ResourceState::CopyDest, nullptr);
        ResourceBarrier(uploadBuffer.get(), ResourceState::CopySource, nullptr);

        auto srcHandle = static_cast<VulkanBuffer *>(uploadBuffer.get())->GetHandle();
        auto dstHandle = static_cast<const VulkanTexture *>(texture)->GetHandle();
        vkCmdCopyBufferToImage(contextData->GetCommandBufferHandle(), srcHandle, dstHandle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

        dataSrc += dataSize;
    }

    commandsPending = true;
}

Array<uint8> VulkanCopyContextImpl::ReadSubresource(const Texture *texture, int32 subresource)
{
    auto task = ReadSubresourceAsync(texture, subresource);
    return task->GetData();
}

SPtr<ReadTextureTask> VulkanCopyContextImpl::ReadSubresourceAsync(const Texture *texture, int32 subresource)
{
    return ReadTextureTask::Create(copyContext, texture, subresource);
}

bool VulkanCopyContextImpl::BufferBarrier(const Buffer *buffer, ResourceState newState)
{
    VkPipelineStageFlags srcStageMask = ToVkPipelineStage(buffer->GetGlobalState(), true);
    VkPipelineStageFlags dstStageMask = ToVkPipelineStage(newState, false);

    if (srcStageMask != dstStageMask)
    {
        VkBufferMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        barrier.srcAccessMask = ToVkAccess(buffer->GetGlobalState());
        barrier.dstAccessMask = ToVkAccess(newState);
        barrier.buffer = static_cast<const VulkanBuffer *>(buffer)->GetHandle();
        barrier.offset = buffer->GetGpuAddressOffset();
        barrier.size = buffer->GetSize();

        vkCmdPipelineBarrier(contextData->GetCommandBufferHandle(), srcStageMask, dstStageMask, 0, 0, nullptr, 1, &barrier, 0, nullptr);

        buffer->SetGlobalState(newState);
        commandsPending = true;
        return true;
    }
    return false;
}

bool VulkanCopyContextImpl::TextureBarrier(const Texture *texture, ResourceState newState)
{
    VkImageLayout srcLayout = ToVkImageLayout(texture->GetGlobalState());
    VkImageLayout dstLayout = ToVkImageLayout(newState);

    if (srcLayout != dstLayout)
    {
        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = srcLayout;
        barrier.newLayout = dstLayout;
        barrier.image = static_cast<const VulkanTexture *>(texture)->GetHandle();
        barrier.subresourceRange.aspectMask = ToVkImageAspect(texture->GetResourceFormat());
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.layerCount = texture->GetArrayLayers();
        barrier.subresourceRange.levelCount = texture->GetMipLevels();
        barrier.srcAccessMask = ToVkAccess(texture->GetGlobalState());
        barrier.dstAccessMask = ToVkAccess(newState);

        VkPipelineStageFlags srcStageMask = ToVkPipelineStage(texture->GetGlobalState(), true);
        VkPipelineStageFlags dstStageMask = ToVkPipelineStage(newState, false);
        vkCmdPipelineBarrier(contextData->GetCommandBufferHandle(), srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        texture->SetGlobalState(newState);
        commandsPending = true;
        return true;
    }
    return false;
}

bool VulkanCopyContextImpl::SubresouceBarriers(const Texture *texture, ResourceState newState, const ResourceViewInfo *viewInfo)
{
    ResourceViewInfo tempViewInfo;
    bool setGlobal = false;
    if (viewInfo == nullptr)
    {
        tempViewInfo.arrayLayers = texture->GetArrayLayers();
        tempViewInfo.firstArraySlice = 0;
        tempViewInfo.mipLevels = texture->GetMipLevels();
        tempViewInfo.mostDetailedMip = 0;
        viewInfo = &tempViewInfo;
        setGlobal = true;
    }

    auto Barrier = [this](const Texture *texture, ResourceState newState, ResourceState oldState, int32 arraySlice, int32 mipLevel) {
        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.newLayout = ToVkImageLayout(newState);
        barrier.oldLayout = ToVkImageLayout(oldState);
        barrier.image = static_cast<const VulkanTexture *>(texture)->GetHandle();
        barrier.subresourceRange.aspectMask = ToVkImageAspect(texture->GetResourceFormat());
        barrier.subresourceRange.baseArrayLayer = arraySlice;
        barrier.subresourceRange.baseMipLevel = mipLevel;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;
        barrier.srcAccessMask = ToVkAccess(oldState);
        barrier.dstAccessMask = ToVkAccess(newState);
        vkCmdPipelineBarrier(contextData->GetCommandBufferHandle(), ToVkPipelineStage(oldState, true), ToVkPipelineStage(newState, false), 0, 0, nullptr, 0, nullptr, 1, &barrier);
    };

    bool entireTransitioned = true;
    for (int32 a = viewInfo->firstArraySlice; a < viewInfo->firstArraySlice + viewInfo->arrayLayers; ++a)
    {
        for (int32 m = viewInfo->mostDetailedMip; m < viewInfo->mostDetailedMip + viewInfo->mipLevels; ++m)
        {
            auto oldState = texture->GetSubresourceState(a, m);
            if (oldState != newState)
            {
                Barrier(texture, newState, oldState, a, m);
                if (!setGlobal)
                {
                    texture->SetSubresourceState(a, m, newState);
                }
                commandsPending = true;
            }
            else
            {
                entireTransitioned = false;
            }
        }
    }

    if (setGlobal)
    {
        texture->SetGlobalState(newState);
    }
    return entireTransitioned;
}
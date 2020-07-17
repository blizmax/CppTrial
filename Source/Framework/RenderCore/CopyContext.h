#pragma once

#include "RenderCore/Buffer.h"
#include "RenderCore/GpuFence.h"
#include "RenderCore/GpuQueue.h"
#include "RenderCore/Texture.h"

class ReadTextureTask
{
public:
    ReadTextureTask() = default;

    Array<uint8> GetData()
    {
        Array<uint8> result;
        fence->SyncCpu();
        result.AddUninitialized(size);
        uint8 *ptr = reinterpret_cast<uint8 *>(buffer->Map(BufferMapType::Read));
        std::memcpy(result.GetData(), ptr, size);
        buffer->Unmap();
        return result;
    }

    static SPtr<ReadTextureTask> Create(class CopyContext *ctx, const Texture *texture, int32 subresource);

private:
    SPtr<GpuFence> fence;
    SPtr<Buffer> buffer;
    uint32 size;
};

class CopyContext
{
public:
    virtual ~CopyContext() = default;

    virtual void Flush(bool wait = false) = 0;

    virtual bool ResourceBarrier(const Resource *resource, ResourceState newState, const ResourceViewInfo *viewInfo = nullptr) = 0;
    virtual void UavBarrier(const Resource *resource) = 0;
    virtual void CopyResource(const Resource *dst, const Resource *src) = 0;
    virtual void CopyBufferRegion(const Buffer *dst, int32 dstOffset, const Buffer *src, int32 srcOffset, uint32 size) = 0;
    virtual void CopySubresource(const Texture *dst, int32 dstSub, const Texture *src, int32 srcSub) = 0;
    virtual void CopySubresourceRegion(const Texture *dst, int32 dstSub, const Texture *src, int32 srcSub, const Vector3I &dstOffset = Vector3I(), const Vector3I &srcOffset = Vector3I(), const Vector3I &size = Vector3I(-1)) = 0;
    virtual void UpdateBuffer(const Buffer *buffer, const void *data, int32 offset, uint32 size) = 0;
    virtual void UpdateTexture(const Texture *texture, const void *data) = 0;
    virtual void UpdateSubresource(const Texture *texture, int32 subresource, const void *data, const Vector3I &offset = Vector3I(), const Vector3I &size = Vector3I(-1)) = 0;
    virtual void UpdateSubresources(const Texture *texture, int32 firstSub, int32 subCount, const void *data) = 0;
    virtual Array<uint8> ReadSubresource(const Texture *texture, int32 subresource) = 0;
    virtual SPtr<ReadTextureTask> ReadSubresourceAsync(const Texture *texture, int32 subresource) = 0;

    static SPtr<CopyContext> Create(const SPtr<GpuQueue> &queue);
};
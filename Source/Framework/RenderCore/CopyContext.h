#pragma once

#include "RenderCore/Buffer.h"
#include "RenderCore/Texture.h"
#include "RenderCore/GpuFence.h"
#include "RenderCore/GpuQueue.h"
#include "Math/Vector3.h"

namespace RenderCore
{
class ReadTextureTask
{
public:
    ReadTextureTask() = default;

    Array<uint8> GetData()
    {
        Array<uint8> result;
        fence->SyncCpu();
        result.AppendUninitialized(size);
        uint8 *ptr = reinterpret_cast<uint8 *>(buffer->Map(BufferMapType::Read));
        std::memcpy(result.GetData(), ptr, size);
        buffer->Unmap();
        return result;
    }

    static SPtr<ReadTextureTask> Create(CopyContext *ctx, const Texture *texture, uint32 subresource);

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
    virtual void CopyBufferRegion(const Buffer *dst, uint32 dstOffset, const Buffer *src, uint32 srcOffset, uint32 size) = 0;
    virtual void CopySubresource(const Texture *dst, uint32 dstSub, const Texture *src, uint32 srcSub) = 0;
    virtual void CopySubresourceRegion(const Texture *dst, uint32 dstSub, const Texture *src, uint32 srcSub, const Vector3 &dstOffset, const Vector3 &srcOffset, const Vector3 &size) = 0;
    virtual void UpdateBuffer(const Buffer *buffer, const void *data, uint32 offset, uint32 size) = 0;
    virtual void UpdateTexture(const Texture *texture, const void *data) = 0;
    virtual void UpdateSubresource(const Texture *texture, uint32 subresource, const void* data, const Vector3 &offset, const Vector3 &size) = 0;
    virtual void UpdateSubresources(const Texture *texture, uint32 firstSub, uint32 subCount, const void* data) = 0;
    virtual Array<uint8> ReadSubresource(const Texture *texture, uint32 subresource) = 0;
    virtual SPtr<ReadTextureTask> ReadSubresourceAsync(const Texture *texture, uint32 subresource) = 0;

    static SPtr<CopyContext> Create(const SPtr<GpuQueue> &queue);

};
}
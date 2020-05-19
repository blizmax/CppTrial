#pragma once

#include "RenderCore/Buffer.h"
#include "RenderCore/Texture.h"
#include "RenderCore/GpuFence.h"
#include "RenderCore/GpuQueue.h"

namespace RenderCore
{
class CopyContext
{
public:
    virtual ~CopyContext() = default;

    virtual bool ResourceBarrier(const Resource *resource, ResourceState newState, const ResourceViewInfo *viewInfo = nullptr) = 0;
    virtual void UavBarrier(const Resource *resource) = 0;
    virtual void CopyResource(const Resource *dst, const Resource *src) = 0;

    virtual void Flush(bool wait = false);

    static SPtr<CopyContext> Create(const SPtr<GpuQueue> &queue);

};
}
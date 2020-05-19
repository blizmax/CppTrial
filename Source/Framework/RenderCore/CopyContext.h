#pragma once

#include "RenderCore/Resource.h"
#include "RenderCore/GpuFence.h"

namespace RenderCore
{
class CopyContext
{
public:
    virtual ~CopyContext() = default;

    virtual bool ResourceBarrier(const Resource *resource, ResourceState newState, const ResourceViewInfo *viewInfo = nullptr);
    virtual void UavBarrier(const Resource *resource);

    virtual void Flush(bool wait = false);

    static SPtr<RenderContext> Create();
};
}
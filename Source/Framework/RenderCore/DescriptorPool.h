#pragma once

#include "RenderCore/GpuFence.h"

namespace RenderCore
{

struct DescriptorPoolDesc
{
    uint32 descCount[(int32)DescriptorType::Count] = {0};
};

class DescriptorPool
{
public:
    virtual ~DescriptorPool() = default;

    const DescriptorPoolDesc &GetDesc() const
    {
        return desc;
    }

    static SPtr<DescriptorPool> Create(const DescriptorPoolDesc &desc, const SPtr<GpuFence> &fence);

protected:
    DescriptorPool(const Desc &desc, const SPtr<GpuFence> &fence)
        : desc(desc), fence(fence)
    {
    }

protected:
    DescriptorPoolDesc desc;
    SPtr<GpuFence> fence;
};
}
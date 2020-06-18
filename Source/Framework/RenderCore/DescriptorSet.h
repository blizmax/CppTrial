#pragma once

#include "RenderCore/DescriptorSetLayout.h"
#include "RenderCore/DescriptorPool.h"

class DescriptorSet
{
public:
    virtual ~DescriptorSet() = default;

    virtual void SetSrv(int32 binding, int32 index, const ResourceView *srv) = 0;
    virtual void SetUav(int32 binding, int32 index, const ResourceView *uav) = 0;
    virtual void SetCbv(int32 binding, int32 index, const ResourceView *cbv) = 0;
    virtual void SetSampler(int32 binding, int32 index, const Sampler *sampler) = 0;

    virtual void BindForGraphics(CopyContext *ctx, const RootSignature *rootSignature, int32 setIndex) = 0;
    virtual void BindForCompute(CopyContext *ctx, const RootSignature *rootSignature, int32 setIndex) = 0;

    static SPtr<DescriptorSet> Create(const SPtr<DescriptorPool> &pool, const SPtr<DescriptorSetLayout> &layout);
};
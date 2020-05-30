#pragma once

#include "RenderCore/DescriptorSetLayout.h"
#include "RenderCore/DescriptorPool.h"

namespace RenderCore
{

class ResourceView;
class Sampler;
class CopyContext;
class RootSignature;

class DescriptorSet
{
public:
    virtual ~DescriptorSet() = default;

    virtual void SetSrv(uint32 binding, uint32 index, const ResourceView *srv) = 0;
    virtual void SetUav(uint32 binding, uint32 index, const ResourceView *uav) = 0;
    virtual void SetCbv(uint32 binding, uint32 index, const ResourceView *cbv) = 0;
    virtual void SetSampler(uint32 binding, uint32 index, const Sampler *sampler) = 0;

    virtual void BindForGraphics(CopyContext *ctx, const RootSignature *rootSignature, uint32 setIndex) = 0;
    virtual void BindForCompute(CopyContext *ctx, const RootSignature *rootSignature, uint32 setIndex) = 0;

    static SPtr<DescriptorSet> Create(const SPtr<DescriptorPool> &pool, const SPtr<DescriptorSetLayout> &layout);
};
}
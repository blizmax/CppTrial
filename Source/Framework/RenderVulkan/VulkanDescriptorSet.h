#pragma once

#include "RenderCore/DescriptorSet.h"
#include "RenderVulkan/VulkanDescriptorPool.h"
#include "RenderVulkan/VulkanDescriptorSetLayout.h"
#include "RenderVulkan/VulkanMemory.h"
#include "RenderVulkan/VulkanResourceView.h"

class VulkanDescriptorSet : public DescriptorSet
{
public:
    VulkanDescriptorSet(const SPtr<DescriptorPool> &pool, const SPtr<DescriptorSetLayout> &layout);
    virtual ~VulkanDescriptorSet();

    virtual void SetSrv(int32 binding, int32 index, const ResourceView *srv) override;
    virtual void SetUav(int32 binding, int32 index, const ResourceView *uav) override;
    virtual void SetCbv(int32 binding, int32 index, const ResourceView *cbv) override;
    virtual void SetSampler(int32 binding, int32 index, const Sampler *sampler) override;

    virtual void BindForGraphics(CopyContext *ctx, const RootSignature *rootSignature, int32 setIndex) override;
    virtual void BindForCompute(CopyContext *ctx, const RootSignature *rootSignature, int32 setIndex) override;

    VkDescriptorSet GetHandle() const
    {
        return descriptorSet;
    }

private:
    void SetBufferView(int32 binding, int32 index, const VulkanBufferView *view);
    void SetImageView(int32 binding, int32 index, const VulkanImageView *view, bool uav);

private:
    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    SPtr<VulkanDescriptorPool> pool;
    SPtr<VulkanDescriptorSetLayout> layout;
};
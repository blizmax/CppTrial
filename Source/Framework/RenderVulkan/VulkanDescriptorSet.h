#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/DescriptorSet.h"
#include "RenderVulkan/VulkanDescriptorPool.h"
#include "RenderVulkan/VulkanDescriptorSetLayout.h"
#include "RenderVulkan/VulkanResourceView.h"

namespace RenderCore
{

class VulkanDescriptorSet : public DescriptorSet
{
public:
    VulkanDescriptorSet(const SPtr<DescriptorPool> &pool, const SPtr<DescriptorSetLayout> &layout);
    virtual ~VulkanDescriptorSet();

    virtual void SetSrv(uint32 binding, uint32 index, const ResourceView *srv) override;
    virtual void SetUav(uint32 binding, uint32 index, const ResourceView *uav) override;
    virtual void SetCbv(uint32 binding, uint32 index, const ResourceView *cbv) override;
    virtual void SetSampler(uint32 binding, uint32 index, const Sampler *sampler) override;

    virtual void BindForGraphics(CopyContext *ctx, const RootSignature *rootSignature, uint32 setIndex) override;
    virtual void BindForCompute(CopyContext *ctx, const RootSignature *rootSignature, uint32 setIndex) override;

    VkDescriptorSet GetHandle() const
    {
        return descriptorSet;
    }

private:
    void SetBufferView(uint32 binding, uint32 index, const VulkanBufferView *view);
    void SetImageView(uint32 binding, uint32 index, const VulkanImageView *view, bool uav);

private:
    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    SPtr<VulkanDescriptorPool> pool;
    SPtr<VulkanDescriptorSetLayout> layout;
};

}
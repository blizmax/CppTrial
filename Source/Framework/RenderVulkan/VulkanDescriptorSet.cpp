#include "RenderVulkan/VulkanDescriptorSet.h"
#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/VulkanBuffer.h"
#include "RenderVulkan/VulkanSampler.h"
#include "RenderVulkan/VulkanCopyContext.h"
#include "RenderVulkan/VulkanRootSignature.h"

namespace RenderCore
{

VulkanDescriptorSet::VulkanDescriptorSet(const SPtr<DescriptorPool> &pool, const SPtr<DescriptorSetLayout> &layout)
{
    this->pool = std::static_pointer_cast<VulkanDescriptorPool>(pool);
    this->layout = std::static_pointer_cast<VulkanDescriptorSetLayout>(layout);

    auto vkLayout = this->layout->GetHandle();
    VkDescriptorSetAllocateInfo setInfo = {};
    setInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    setInfo.descriptorPool = this->pool->GetHandle();
    setInfo.descriptorSetCount = 1;
    setInfo.pSetLayouts = &vkLayout;
    if(vkAllocateDescriptorSets(gVulkanDevice->GetLogicalDeviceHandle(), &setInfo, &descriptorSet) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Alloc descriptor set failed.");
}

VulkanDescriptorSet::~VulkanDescriptorSet()
{
    if(descriptorSet != VK_NULL_HANDLE)
    {
        vkFreeDescriptorSets(gVulkanDevice->GetLogicalDeviceHandle(), pool->GetHandle(), 1, &descriptorSet);
        descriptorSet = VK_NULL_HANDLE;
    }
}

void VulkanDescriptorSet::SetSrv(uint32 binding, uint32 index, const ResourceView *srv)
{
    CT_CHECK(srv);

    const VulkanBufferView *bufferView = dynamic_cast<const VulkanBufferView *>(srv);
    if(bufferView)
    {
        SetBufferView(binding, index, bufferView);
    }
    else
    {
        const VulkanImageView *imageView = dynamic_cast<const VulkanImageView *>(srv);
        SetImageView(binding, index, imageView, false);
    }
}

void VulkanDescriptorSet::SetUav(uint32 binding, uint32 index, const ResourceView *uav)
{
    CT_CHECK(uav);

    const VulkanBufferView *bufferView = dynamic_cast<const VulkanBufferView *>(uav);
    if(bufferView)
    {
        SetBufferView(binding, index, bufferView);
    }
    else
    {
        const VulkanImageView *imageView = dynamic_cast<const VulkanImageView *>(uav);
        SetImageView(binding, index, imageView, true);
    }
}

void VulkanDescriptorSet::SetCbv(uint32 binding, uint32 index, const ResourceView *cbv)
{
    CT_CHECK(cbv);

    const VulkanBufferView *bufferView = static_cast<const VulkanBufferView *>(cbv);
    SetBufferView(binding, index, bufferView);
}

void VulkanDescriptorSet::SetSampler(uint32 binding, uint32 index, const Sampler *sampler)
{
    CT_CHECK(sampler);

    VkDescriptorImageInfo info = {};
    info.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    info.imageView = nullptr;
    info.sampler = static_cast<const VulkanSampler *>(sampler)->GetHandle();

    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = descriptorSet;
    write.dstBinding = binding;
    write.dstArrayElement = index;
    write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    write.descriptorCount = 1;
    write.pImageInfo = &info;

    vkUpdateDescriptorSets(gVulkanDevice->GetLogicalDeviceHandle(), 1, &write, 0, nullptr);
}

void VulkanDescriptorSet::BindForGraphics(CopyContext *ctx, const RootSignature *rootSignature, uint32 setIndex)
{
    auto vkCtx = static_cast<VulkanCopyContext *>(ctx);
    auto commandBuffer = vkCtx->GetContextData()->GetCommandBufferHandle();
    auto vkRootSig = static_cast<const VulkanRootSignature *>(rootSignature);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkRootSig->GetHandle(), setIndex, 1, &descriptorSet, 0, nullptr);
}

void VulkanDescriptorSet::BindForCompute(CopyContext *ctx, const RootSignature *rootSignature, uint32 setIndex)
{
    auto vkCtx = static_cast<VulkanCopyContext *>(ctx);
    auto commandBuffer = vkCtx->GetContextData()->GetCommandBufferHandle();
    auto vkRootSig = static_cast<const VulkanRootSignature *>(rootSignature);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, vkRootSig->GetHandle(), setIndex, 1, &descriptorSet, 0, nullptr);
}

void VulkanDescriptorSet::SetBufferView(uint32 binding, uint32 index, const VulkanBufferView *view)
{
    auto buffer = static_cast<const VulkanBuffer *>(view->GetResource());

    VkDescriptorBufferInfo info = {};
    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = descriptorSet;
    write.dstBinding = binding;
    write.dstArrayElement = index;
    write.descriptorType = ToVkDescriptorType(layout->GetElements()[binding].descriptorType);
    write.descriptorCount = 1;
    
    if(buffer->IsTyped())
    {
        auto uav = static_cast<VulkanBufferView *>(buffer->GetUav().get())->GetHandle();
        write.pTexelBufferView = &uav;
    }
    else
    {
        info.buffer = buffer->GetHandle();
        info.offset = buffer->GetOffset();
        info.range = buffer->GetSize();
        write.pBufferInfo = &info;
    }

    vkUpdateDescriptorSets(gVulkanDevice->GetLogicalDeviceHandle(), 1, &write, 0, nullptr);
}

void VulkanDescriptorSet::SetImageView(uint32 binding, uint32 index, const VulkanImageView *view, bool uav)
{
    auto buffer = static_cast<const VulkanBuffer *>(view->GetResource());

    VkDescriptorImageInfo info = {};
    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = descriptorSet;
    write.dstBinding = binding;
    write.dstArrayElement = index;
    write.descriptorType = ToVkDescriptorType(layout->GetElements()[binding].descriptorType);
    write.descriptorCount = 1;
    
    info.imageLayout = uav ? VK_IMAGE_LAYOUT_GENERAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    info.imageView = view->GetHandle();
    info.sampler = nullptr;
    write.pImageInfo = &info;

    vkUpdateDescriptorSets(gVulkanDevice->GetLogicalDeviceHandle(), 1, &write, 0, nullptr);
}

}
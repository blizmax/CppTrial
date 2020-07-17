#include "RenderVulkan/VulkanSampler.h"
#include "RenderVulkan/VulkanDevice.h"

SPtr<Sampler> Sampler::Create(const SamplerDesc &desc)
{
    return Memory::MakeShared<VulkanSampler>(desc);
}

VulkanSampler::VulkanSampler(const SamplerDesc &desc)
    : Sampler(desc)
{
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = ToVkFilter(desc.magFilter);
    samplerInfo.minFilter = ToVkFilter(desc.minFilter);
    samplerInfo.mipmapMode = ToVkMipFilter(desc.mipFilter);
    samplerInfo.addressModeU = ToVkAddressMode(desc.uWrap);
    samplerInfo.addressModeV = ToVkAddressMode(desc.vWrap);
    samplerInfo.addressModeW = ToVkAddressMode(desc.wWrap);
    samplerInfo.mipLodBias = desc.lodBias;
    samplerInfo.anisotropyEnable = desc.maxAnisotropy > 1;
    samplerInfo.maxAnisotropy = (float)desc.maxAnisotropy;
    samplerInfo.compareEnable = desc.compareOp != CompareOperation::AlwaysPass;
    samplerInfo.compareOp = ToVkCompareOperation(desc.compareOp);
    samplerInfo.minLod = desc.minLod;
    samplerInfo.maxLod = desc.maxLod;
    samplerInfo.borderColor = ToVkBorderColor(desc.borderColor);
    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    if (vkCreateSampler(gVulkanDevice->GetLogicalDeviceHandle(), &samplerInfo, gVulkanAlloc, &sampler) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create sampler failed.");
}

VulkanSampler::~VulkanSampler()
{
    if (sampler != VK_NULL_HANDLE)
    {
        if (gVulkanDevice)
        {
            gVulkanDevice->Release([sampler = sampler]() {
                vkDestroySampler(gVulkanDevice->GetLogicalDeviceHandle(), sampler, gVulkanAlloc);
            });
        }
        sampler = VK_NULL_HANDLE;
    }
}
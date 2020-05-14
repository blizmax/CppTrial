#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/RootSignature.h"

namespace RenderCore
{
class VulkanRootSignature : public RootSignature
{
public:
    ~VulkanRootSignature();

    VkPipelineLayout GetHandle() const
    {
        return pipelineLayout;
    }

protected:
    VulkanRootSignature(const RootSignatureDesc &desc);

private:
    Array<VkDescriptorSetLayout> setLayouts;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
};

}
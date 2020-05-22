#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/RootSignature.h"

namespace RenderCore
{
class VulkanRootSignature : public RootSignature
{
public:
    VulkanRootSignature(const RootSignatureDesc &desc);
    ~VulkanRootSignature();

    VkPipelineLayout GetHandle() const
    {
        return pipelineLayout;
    }

private:
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
};

}
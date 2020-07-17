#pragma once

#include "RenderCore/RootSignature.h"
#include "RenderVulkan/VulkanMemory.h"

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
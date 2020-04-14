#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

struct VulkanShaderCreateParams
{
    Array<uchar8> vertexSource;
    Array<uchar8> fragmentSource;
};

class VulkanShader : public VulkanResource
{
public:
    VulkanShader(const VulkanShaderCreateParams &params);
    virtual void Destroy() override;

    VkShaderModule GetVertexModuleHandle() const
    {
        return vertexModule;
    }

    VkShaderModule GetFragmentModuleHandle() const
    {
        return fragmentModule;
    }

    static SPtr<VulkanShader> Create(const VulkanShaderCreateParams &params);

private:
    VkShaderModule CreateShaderModule(const Array<uchar8> &code);

private:
    VkShaderModule vertexModule = VK_NULL_HANDLE;
    VkShaderModule fragmentModule = VK_NULL_HANDLE;
};
}
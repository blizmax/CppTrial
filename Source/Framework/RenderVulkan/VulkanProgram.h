#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/Program.h"

namespace RenderCore
{
class VulkanProgramKernel : public ProgramKernel
{
public:
    struct ShaderData
    {
        ShaderType shaderType;
        VkShaderModule module = VK_NULL_HANDLE;
    };

    VulkanProgramKernel(const ProgramDesc &desc);
    virtual ~VulkanProgramKernel();

    VkShaderModule GetShaderModuleHandle(ShaderType shaderType) const
    {
        for(const auto &e : shaderDatas)
        {
            if(e.shaderType == shaderType)
                return e.module;
        }
        return VK_NULL_HANDLE;
    }

private:
    Array<ShaderData> shaderDatas;
};

}
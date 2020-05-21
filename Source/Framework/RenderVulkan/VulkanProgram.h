#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/Program.h"

namespace RenderCore
{
class VulkanProgram : public Program
{
public:
    struct ShaderData
    {
        ShaderType shaderType;
        VkShaderModule module = VK_NULL_HANDLE;
    };

    VulkanProgram(const ProgramDesc &desc);
    virtual ~VulkanProgram();

private:
    ShaderData CompileShader(const ShaderDesc &desc);

private:
    Array<ShaderData> shaderDatas;
};

}
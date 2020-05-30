#include "RenderVulkan/VulkanProgram.h"
#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/Private/VulkanShaderCompiler.h"

namespace RenderCore
{
VulkanProgram::VulkanProgram(const ProgramDesc &desc) : Program(desc)
{

}

VulkanProgram::~VulkanProgram()
{

}

VulkanProgram::ShaderData VulkanProgram::CompileShader(const ShaderDesc &desc)
{
    // VkShaderModuleCreateInfo createInfo = {};
    // createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    // createInfo.pNext = nullptr;
    // createInfo.flags = 0;
    // createInfo.codeSize = code.Count();
    // createInfo.pCode = reinterpret_cast<const uint32 *>(code.GetData());

    return {};
}

}
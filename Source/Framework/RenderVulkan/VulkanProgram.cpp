#include "RenderVulkan/VulkanProgram.h"
#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/Private/VulkanShaderCompiler.h"

namespace RenderCore
{
SPtr<ProgramKernel> ProgramKernel::Create(const ProgramDesc &desc)
{
    return Memory::MakeShared<VulkanProgramKernel>(desc);
}

VulkanProgramKernel::VulkanProgramKernel(const ProgramDesc &desc) : ProgramKernel(desc)
{
    for(const auto &d : desc.shaderDescs)
    {
        shaderDatas.Add(CompileShader(d));
    }

    //TODO reflection
    RootSignatureDesc rootSignatureDesc;
    rootSignature = RootSignature::Create(rootSignatureDesc);
}

VulkanProgramKernel::~VulkanProgramKernel()
{
    for(const auto &d : shaderDatas)
    {
        vkDestroyShaderModule(gVulkanDevice->GetLogicalDeviceHandle(), d.module, gVulkanAlloc);
    }
    shaderDatas.Clear();
}

VulkanProgramKernel::ShaderData VulkanProgramKernel::CompileShader(const ShaderDesc &desc)
{
    auto code = gVulkanShaderCompiler->Compile(desc.shaderType, desc.source);

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.Count();
    createInfo.pCode = reinterpret_cast<const uint32 *>(code.GetData());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(gVulkanDevice->GetLogicalDeviceHandle(), &createInfo, gVulkanAlloc, &shaderModule) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create shader module failed.");

    return {desc.shaderType, shaderModule};
}

}
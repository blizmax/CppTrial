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
    reflection = ProgramReflection::Create();

    for(const auto &d : desc.shaderDescs)
    {
        shaderDatas.Add(CompileShader(d));
    }

    rootSignature = RootSignature::Create(reflection->GetRootSignatureDesc());
}

VulkanProgramKernel::~VulkanProgramKernel()
{
    for (const auto &d : shaderDatas)
    {
        if(gVulkanDevice)
        {
            gVulkanDevice->Release([module = d.module]() {
                vkDestroyShaderModule(gVulkanDevice->GetLogicalDeviceHandle(), module, gVulkanAlloc);
            });
        }
    }
    shaderDatas.Clear();
}

VulkanProgramKernel::ShaderData VulkanProgramKernel::CompileShader(const ShaderDesc &desc)
{
    auto code = gVulkanShaderCompiler->Compile(desc.shaderType, desc.source, reflection);

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
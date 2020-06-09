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
    auto CreateShaderModule = [this](ShaderType shaderType, const Array<uchar8>& code)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.Count();
        createInfo.pCode = reinterpret_cast<const uint32 *>(code.GetData());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(gVulkanDevice->GetLogicalDeviceHandle(), &createInfo, gVulkanAlloc, &shaderModule) != VK_SUCCESS)
            CT_EXCEPTION(RenderCore, "Create shader module failed.");

        this->shaderDatas.Add({shaderType, shaderModule});
    };

    ProgramReflectionBuilder builder;
    gVulkanShaderCompiler->Compile(desc, CreateShaderModule, builder);
    builder.Build();

    reflection = builder.GetReflection();
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
}
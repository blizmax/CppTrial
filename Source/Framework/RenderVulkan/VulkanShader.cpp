#include "RenderVulkan/VulkanShader.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
SPtr<Shader> Shader::Create(const ShaderCreateParams &params)
{
    auto result = Memory::MakeShared<VulkanShader>(params);
    VulkanContext::Get().GetShaderRegistry().Register(result.get());
    return result;
}

VulkanShader::VulkanShader(const ShaderCreateParams &params)
{
    vertexModule = CreateShaderModule(params.vertexSource);
    fragmentModule = CreateShaderModule(params.fragmentSource);
}

VulkanShader::~VulkanShader()
{
    VulkanContext::Get().GetShaderRegistry().Remove(this);
}

void VulkanShader::Destroy()
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();

    if(vertexModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(device, vertexModule, gVulkanAlloc);
        vertexModule = VK_NULL_HANDLE;
    }
    if (fragmentModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(device, fragmentModule, gVulkanAlloc);
        vertexModule = VK_NULL_HANDLE;
    }
}

VkShaderModule VulkanShader::CreateShaderModule(const Array<uchar8> &code)
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.codeSize = code.Count();
    createInfo.pCode = reinterpret_cast<const uint32 *>(code.GetData());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, gVulkanAlloc, &shaderModule) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create shader module failed.");

    return shaderModule;
}
}
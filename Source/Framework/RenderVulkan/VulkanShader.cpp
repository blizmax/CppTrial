#include "RenderVulkan/VulkanShader.h"
#include "RenderVulkan/VulkanContext.h"

namespace RenderCore
{
SPtr<VulkanShader> VulkanShader::Create(const VulkanShaderCreateParams &params)
{
    return Memory::MakeShared<VulkanShader>(params);
}

VulkanShader::VulkanShader(const VulkanShaderCreateParams &params)
{
    vertexModule = CreateShaderModule(params.vertexSource);
    fragmentModule = CreateShaderModule(params.fragmentSource);
}

void VulkanShader::Destroy()
{
    auto &device = VulkanContext::Get().GetDevice();
    auto logicalDevice = device->GetLogicalDeviceHandle();

    if(vertexModule != VK_NULL_HANDLE)
        vkDestroyShaderModule(logicalDevice, vertexModule, gVulkanAlloc);
    if (fragmentModule != VK_NULL_HANDLE)
        vkDestroyShaderModule(logicalDevice, fragmentModule, gVulkanAlloc);
}

VkShaderModule VulkanShader::CreateShaderModule(const Array<uchar8> &code)
{
    auto &device = VulkanContext::Get().GetDevice();
    auto logicalDevice = device->GetLogicalDeviceHandle();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.codeSize = code.Count();
    createInfo.pCode = reinterpret_cast<const uint32 *>(code.GetData());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(logicalDevice, &createInfo, gVulkanAlloc, &shaderModule) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create shader module failed.");

    return shaderModule;
}
}
#include "RenderVulkan/VulkanComputeStateObject.h"
#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/VulkanProgram.h"
#include "RenderVulkan/VulkanRootSignature.h"

SPtr<ComputeStateObject> ComputeStateObject::Create(const ComputeStateObjectDesc &desc)
{
    return Memory::MakeShared<VulkanComputeStateObject>(desc);
}

VulkanComputeStateObject::VulkanComputeStateObject(const ComputeStateObjectDesc &desc)
    : ComputeStateObject(desc)
{
    auto vkProgramKernel = static_cast<VulkanProgramKernel *>(desc.programKernel.get());
    auto handle = vkProgramKernel->GetShaderModuleHandle(ShaderType::Compute);
    CT_CHECK(handle != VK_NULL_HANDLE);
    VkPipelineShaderStageCreateInfo stageInfo = {};
    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.stage = ToVkShaderStage(ShaderType::Compute);
    stageInfo.module = handle;
    stageInfo.pName = "main";
    stageInfo.pSpecializationInfo = nullptr;

    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage = stageInfo;
    pipelineInfo.layout = std::static_pointer_cast<VulkanRootSignature>(desc.rootSignature)->GetHandle();

    if (vkCreateComputePipelines(gVulkanDevice->GetLogicalDeviceHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, gVulkanAlloc, &pipeline) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create compute pipeline failed.");
}

VulkanComputeStateObject::~VulkanComputeStateObject()
{
    if (pipeline != VK_NULL_HANDLE)
    {
        if (gVulkanDevice)
        {
            gVulkanDevice->Release([pipeline = pipeline]() {
                vkDestroyPipeline(gVulkanDevice->GetLogicalDeviceHandle(), pipeline, gVulkanAlloc);
            });
        }
        pipeline = VK_NULL_HANDLE;
    }
}
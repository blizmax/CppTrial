#include "RenderVulkan/VulkanRenderPipeline.h"
#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/VulkanShader.h"
#include "RenderVulkan/VulkanFrameBuffer.h"
#include "RenderVulkan/VulkanRenderPass.h"

namespace RenderCore
{
VulkanRenderPipeline::VulkanRenderPipeline(const RenderPipelineCreateParams &params)
{
    auto &context = VulkanContext::Get();
    auto device = context.GetLogicalDeviceHandle();

    auto shader = std::static_pointer_cast<VulkanShader>(params.shader);
    auto rasterizationState = params.rasterizationState;
    auto blendState = params.blendState;
    auto depthStencilState = params.depthStencilState;

    int32 shaderStageCount = 2;

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.pNext = nullptr;
    vertShaderStageInfo.flags = 0;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = shader->GetVertexModuleHandle();
    vertShaderStageInfo.pName = "main";
    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.pNext = nullptr;
    fragShaderStageInfo.flags = 0;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = shader->GetFragmentModuleHandle();
    fragShaderStageInfo.pName = "main";
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    //TODO
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.pNext = nullptr;
    vertexInputInfo.flags = 0;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.pNext = nullptr;
    inputAssemblyInfo.flags = 0;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    // VkViewport viewport;
    // viewport.x = 0.0f;
    // viewport.y = 0.0f;
    // viewport.width = (float)swapChainExtent.width;
    // viewport.height = (float)swapChainExtent.height;
    // viewport.minDepth = 0.0f;
    // viewport.maxDepth = 1.0f;
    // vkCmdSetViewport(buffer, 0, 1, &viewport);

    // Command
    // VkRect2D scissor;
    // scissor.offset.x = 0.0f;
    // scissor.offset.y = 0.0f;
    // scissor.extent.width = swapChainExtent.width;
    // scissor.extent.height = swapChainExtent.height;
    // vkCmdSetScissor(buffer, 0, 1, &scissor);

    VkPipelineViewportStateCreateInfo viewportInfo = {};
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.pNext = nullptr;
    viewportInfo.flags = 0;
    viewportInfo.viewportCount = 1;
    viewportInfo.pViewports = nullptr; //TODO Use command
    viewportInfo.scissorCount = 1;
    viewportInfo.pScissors = nullptr; //TODO Use command

    auto rasterizationData = rasterizationState->GetData();
    VkPipelineRasterizationStateCreateInfo rasterizationInfo = {};
    rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationInfo.pNext = nullptr;
    rasterizationInfo.flags = 0;
    rasterizationInfo.depthClampEnable = rasterizationData.depthClampEnabled;
    rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizationInfo.polygonMode = ToVkPolygonMode(rasterizationData.polygonMode);
    rasterizationInfo.lineWidth = 1.0f;
    rasterizationInfo.cullMode = ToVkCullMode(rasterizationData.cullMode);
    rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationInfo.depthBiasEnable = rasterizationData.depthBiasEnabled;
    rasterizationInfo.depthBiasConstantFactor = rasterizationData.depthBias;
    rasterizationInfo.depthBiasSlopeFactor = rasterizationData.slopeScaledDepthBias;
    rasterizationInfo.depthBiasClamp = rasterizationData.depthBiasClamp;

    VkPipelineMultisampleStateCreateInfo multisampleInfo = {};
    multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleInfo.pNext = nullptr;
    multisampleInfo.flags = 0;
    multisampleInfo.sampleShadingEnable = VK_FALSE;
    multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; //TODO 1bit means no sampling.
    multisampleInfo.minSampleShading = 1.0f;
    multisampleInfo.pSampleMask = nullptr;
    multisampleInfo.alphaToCoverageEnable = VK_FALSE; //TODO
    multisampleInfo.alphaToOneEnable = VK_FALSE;

    auto depthStencilData = depthStencilState->GetData();
    VkStencilOpState stencilFront;
    stencilFront.compareOp = ToVkCompareOperation(depthStencilData.frontStencilCompareOp);
    stencilFront.depthFailOp = ToVkStencilOperation(depthStencilData.frontStencilZFailOp);
    stencilFront.passOp = ToVkStencilOperation(depthStencilData.frontStencilZFailOp);
    stencilFront.failOp = ToVkStencilOperation(depthStencilData.frontStencilZFailOp);
    stencilFront.reference = 0; //TODO
    stencilFront.compareMask = depthStencilData.stencilReadMask;
    stencilFront.writeMask = depthStencilData.stencilWriteMask;
    VkStencilOpState stencilBack;
    stencilBack.compareOp = ToVkCompareOperation(depthStencilData.backStencilCompareOp);
    stencilBack.depthFailOp = ToVkStencilOperation(depthStencilData.backStencilZFailOp);
    stencilBack.passOp = ToVkStencilOperation(depthStencilData.backStencilZFailOp);
    stencilBack.failOp = ToVkStencilOperation(depthStencilData.backStencilZFailOp);
    stencilBack.reference = 0; //TODO
    stencilBack.compareMask = depthStencilData.stencilReadMask;
    stencilBack.writeMask = depthStencilData.stencilWriteMask;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
    depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilInfo.pNext = nullptr;
    depthStencilInfo.flags = 0;
    depthStencilInfo.depthBoundsTestEnable = false;
    depthStencilInfo.minDepthBounds = 0.0f;
    depthStencilInfo.maxDepthBounds = 1.0f;
    depthStencilInfo.depthTestEnable = depthStencilData.depthReadEnabled;
    depthStencilInfo.depthWriteEnable = depthStencilData.depthWriteEnabled;
    depthStencilInfo.depthCompareOp = ToVkCompareOperation(depthStencilData.depthCompareOp);
    depthStencilInfo.front = stencilFront;
    depthStencilInfo.back = stencilBack;
    depthStencilInfo.stencilTestEnable = depthStencilData.stencilEnabled;

    auto blendData = blendState->GetData();
    VkPipelineColorBlendAttachmentState colorBlendAttachments[CT_COLOR_ATTCHMENT_MAX_NUM];
    for(int32 i = 0; i < CT_COLOR_ATTCHMENT_MAX_NUM; ++i)
    {
        auto &attach = blendData.attachments[i];
        VkPipelineColorBlendAttachmentState blendAttachment = {};
        blendAttachment.colorWriteMask = attach.writeMask;
        blendAttachment.blendEnable = attach.enabled;
        blendAttachment.srcColorBlendFactor = ToVkBlendFactor(attach.srcFactor);
        blendAttachment.dstColorBlendFactor = ToVkBlendFactor(attach.dstFactor);
        blendAttachment.colorBlendOp = ToVkBlendOperation(attach.blendOp);
        blendAttachment.srcAlphaBlendFactor = ToVkBlendFactor(attach.srcAlphaFactor);
        blendAttachment.dstAlphaBlendFactor = ToVkBlendFactor(attach.dstAlphaFactor);
        blendAttachment.alphaBlendOp = ToVkBlendOperation(attach.alphaBlendOp);
    }
    VkPipelineColorBlendStateCreateInfo blendInfo = {};
    blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blendInfo.logicOpEnable = VK_FALSE;
    blendInfo.logicOp = VK_LOGIC_OP_NO_OP;
    blendInfo.attachmentCount = 1; //TODO
    blendInfo.pAttachments = colorBlendAttachments;
    blendInfo.blendConstants[0] = 0.0f;
    blendInfo.blendConstants[1] = 0.0f;
    blendInfo.blendConstants[2] = 0.0f;
    blendInfo.blendConstants[3] = 0.0f;

    auto dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_STENCIL_REFERENCE
    };
    VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
    dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateInfo.pNext = nullptr;
    dynamicStateInfo.flags = 0;
    dynamicStateInfo.dynamicStateCount = dynamicStates.size();
    dynamicStateInfo.pDynamicStates = dynamicStates.begin();


    //TEMP
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = 0;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, gVulkanAlloc, &pipelineLayout) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create pipeline layout failed");

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.flags = 0;
    pipelineInfo.stageCount = shaderStageCount;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportInfo;
    pipelineInfo.pRasterizationState = &rasterizationInfo;
    pipelineInfo.pMultisampleState = &multisampleInfo;
    pipelineInfo.pColorBlendState = &blendInfo;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = context.GetFrameBuffer()->GetRenderPass()->GetHandle();
    pipelineInfo.subpass = 0; //subpass index
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; //TODO
    pipelineInfo.basePipelineIndex = -1; //TODO
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, gVulkanAlloc, &pipeline) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create render pipeline failed.");
}

void VulkanRenderPipeline::Destroy()
{
    auto device = VulkanContext::Get().GetLogicalDeviceHandle();

    if (pipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(device, pipeline, gVulkanAlloc);
        pipeline = VK_NULL_HANDLE;
    }

    //TEMP
    if (pipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(device, pipelineLayout, gVulkanAlloc);
        pipelineLayout = VK_NULL_HANDLE;
    }
}

}
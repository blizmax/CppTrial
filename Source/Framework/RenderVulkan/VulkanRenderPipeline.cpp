#include "RenderVulkan/VulkanRenderPipeline.h"
#include "RenderVulkan/VulkanContext.h"
#include "RenderVulkan/VulkanShader.h"

namespace RenderCore
{
VulkanRenderPipeline::VulkanRenderPipeline(const RenderPipelineCreateParams &params)
{
    auto &device = VulkanContext::Get().GetDevice();
    auto logicalDevice = device->GetLogicalDeviceHandle();
    auto shader = std::static_pointer_cast<VulkanShader>(params.shader);
    auto rasterizationState = params.rasterizationState;
    auto blendState = params.blendState;
    auto depthStencilState = params.depthStencilState;

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = shader->GetVertexModuleHandle();
    vertShaderStageInfo.pName = "main";
    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = shader->GetFragmentModuleHandle();
    fragShaderStageInfo.pName = "main";
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.pNext = nullptr;
    inputAssembly.flags = 0;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Command
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

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    inputAssembly.pNext = nullptr;
    inputAssembly.flags = 0;
    viewportState.viewportCount = 1;
    viewportState.pViewports = nullptr; // Use command
    viewportState.scissorCount = 1;
    viewportState.pScissors = nullptr;  // Use command

    auto rasterizationData = rasterizationState->GetData();
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.pNext = nullptr;
    rasterizer.flags = 0;
    rasterizer.depthClampEnable = rasterizationData.depthClampEnabled;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = ToVkPolygonMode(rasterizationData.polygonMode);
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = ToVkCullMode(rasterizationData.cullMode);
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = rasterizationData.depthBiasEnabled;
    rasterizer.depthBiasConstantFactor = rasterizationData.depthBias;
    rasterizer.depthBiasSlopeFactor = rasterizationData.slopeScaledDepthBias;
    rasterizer.depthBiasClamp = rasterizationData.depthBiasClamp;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.pNext = nullptr;
    multisampling.flags = 0;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; //TODO 1bit means no sampling.
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE; //TODO
    multisampling.alphaToOneEnable = VK_FALSE;

    auto depthStencilData = depthStencilState->GetData();
    VkStencilOpState stencilFrontInfo;
    stencilFrontInfo.compareOp = ToVkCompareOperation(depthStencilData.frontStencilCompareOp);
    stencilFrontInfo.depthFailOp = ToVkStencilOperation(depthStencilData.frontStencilZFailOp);
    stencilFrontInfo.passOp = ToVkStencilOperation(depthStencilData.frontStencilZFailOp);
    stencilFrontInfo.failOp = ToVkStencilOperation(depthStencilData.frontStencilZFailOp);
    stencilFrontInfo.reference = 0; //TODO
    stencilFrontInfo.compareMask = depthStencilData.stencilReadMask;
    stencilFrontInfo.writeMask = depthStencilData.stencilWriteMask;
    VkStencilOpState stencilBackInfo;
    stencilBackInfo.compareOp = ToVkCompareOperation(depthStencilData.backStencilCompareOp);
    stencilBackInfo.depthFailOp = ToVkStencilOperation(depthStencilData.backStencilZFailOp);
    stencilBackInfo.passOp = ToVkStencilOperation(depthStencilData.backStencilZFailOp);
    stencilBackInfo.failOp = ToVkStencilOperation(depthStencilData.backStencilZFailOp);
    stencilBackInfo.reference = 0; //TODO
    stencilBackInfo.compareMask = depthStencilData.stencilReadMask;
    stencilBackInfo.writeMask = depthStencilData.stencilWriteMask;
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
    depthStencilInfo.front = stencilFrontInfo;
    depthStencilInfo.back = stencilBackInfo;
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
    VkPipelineColorBlendStateCreateInfo blendStateInfo = {};
    blendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blendStateInfo.logicOpEnable = VK_FALSE;
    blendStateInfo.logicOp = VK_LOGIC_OP_NO_OP;
    blendStateInfo.attachmentCount = 1; //TODO
    blendStateInfo.pAttachments = colorBlendAttachments;
    blendStateInfo.blendConstants[0] = 0.0f;
    blendStateInfo.blendConstants[1] = 0.0f;
    blendStateInfo.blendConstants[2] = 0.0f;
    blendStateInfo.blendConstants[3] = 0.0f;

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
    if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, gVulkanAlloc, &pipelineLayout) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create pipeline layout failed");

    //TODO
    //Create pipeline

}

void VulkanRenderPipeline::Destroy()
{
    //TODO
}

}
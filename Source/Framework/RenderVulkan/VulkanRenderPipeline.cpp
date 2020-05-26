// #include "RenderVulkan/VulkanRenderPipeline.h"
// #include "RenderVulkan/VulkanContext.h"
// #include "RenderVulkan/VulkanShader.h"
// #include "RenderVulkan/VulkanFrameBuffer.h"
// #include "RenderVulkan/VulkanRenderPass.h"

// namespace RenderCore
// {

// SPtr<RenderPipelineState> RenderPipelineState::Create(const RenderPipelineStateCreateParams &params)
// {
//     return Memory::MakeShared<VulkanRenderPipelineState>(params);
// }

// VulkanRenderPipelineState::VulkanRenderPipelineState(const RenderPipelineStateCreateParams &params)
//     : shader(std::static_pointer_cast<VulkanShader>(params.shader))
// {
//     auto rasterizationState = params.rasterizationState;
//     auto blendState = params.blendState;
//     auto depthStencilState = params.depthStencilState;

//     vertexShaderStageInfo = {};
//     vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//     vertexShaderStageInfo.pNext = nullptr;
//     vertexShaderStageInfo.flags = 0;
//     vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
//     vertexShaderStageInfo.module = shader->GetVertexModuleHandle();
//     vertexShaderStageInfo.pName = "main";
//     vertexShaderStageInfo.pSpecializationInfo = nullptr;
//     fragmentShaderStageInfo = {};
//     fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//     fragmentShaderStageInfo.pNext = nullptr;
//     fragmentShaderStageInfo.flags = 0;
//     fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//     fragmentShaderStageInfo.module = shader->GetFragmentModuleHandle();
//     fragmentShaderStageInfo.pName = "main";
//     fragmentShaderStageInfo.pSpecializationInfo = nullptr;

//     inputAssemblyInfo = {};
//     inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//     inputAssemblyInfo.pNext = nullptr;
//     inputAssemblyInfo.flags = 0;
//     inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//     inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

//     viewportInfo = {};
//     viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//     viewportInfo.pNext = nullptr;
//     viewportInfo.flags = 0;
//     viewportInfo.viewportCount = 1;
//     viewportInfo.pViewports = nullptr; //Dynamic
//     viewportInfo.scissorCount = 1;
//     viewportInfo.pScissors = nullptr; //Dynamic

//     rasterizationInfo = {};
//     const auto &rasterizationDesc = rasterizationState->GetDesc();
//     rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//     rasterizationInfo.pNext = nullptr;
//     rasterizationInfo.flags = 0;
//     rasterizationInfo.depthClampEnable = rasterizationDesc.depthClampEnabled;
//     rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
//     rasterizationInfo.polygonMode = ToVkPolygonMode(rasterizationDesc.polygonMode);
//     rasterizationInfo.lineWidth = 1.0f;
//     rasterizationInfo.cullMode = ToVkCullMode(rasterizationDesc.cullMode);
//     rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
//     rasterizationInfo.depthBiasEnable = rasterizationDesc.depthBiasEnabled;
//     rasterizationInfo.depthBiasConstantFactor = rasterizationDesc.depthBias;
//     rasterizationInfo.depthBiasSlopeFactor = rasterizationDesc.slopeScaledDepthBias;
//     rasterizationInfo.depthBiasClamp = rasterizationDesc.depthBiasClamp;

//     multisampleInfo = {};
//     multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//     multisampleInfo.pNext = nullptr;
//     multisampleInfo.flags = 0;
//     multisampleInfo.sampleShadingEnable = VK_FALSE;
//     multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; //TODO 1bit means no sampling.
//     multisampleInfo.minSampleShading = 1.0f;
//     multisampleInfo.pSampleMask = nullptr;
//     multisampleInfo.alphaToCoverageEnable = VK_FALSE; //TODO
//     multisampleInfo.alphaToOneEnable = VK_FALSE;

//     depthStencilInfo = {};
//     const auto &depthStencilDesc = depthStencilState->GetDesc();
//     VkStencilOpState stencilFront = {};
//     stencilFront.compareOp = ToVkCompareOperation(depthStencilDesc.frontStencilCompareOp);
//     stencilFront.depthFailOp = ToVkStencilOperation(depthStencilDesc.frontStencilZFailOp);
//     stencilFront.passOp = ToVkStencilOperation(depthStencilDesc.frontStencilZFailOp);
//     stencilFront.failOp = ToVkStencilOperation(depthStencilDesc.frontStencilZFailOp);
//     stencilFront.reference = 0; //Dynamic
//     stencilFront.compareMask = depthStencilDesc.stencilReadMask;
//     stencilFront.writeMask = depthStencilDesc.stencilWriteMask;
//     VkStencilOpState stencilBack = {};
//     stencilBack.compareOp = ToVkCompareOperation(depthStencilDesc.backStencilCompareOp);
//     stencilBack.depthFailOp = ToVkStencilOperation(depthStencilDesc.backStencilZFailOp);
//     stencilBack.passOp = ToVkStencilOperation(depthStencilDesc.backStencilZFailOp);
//     stencilBack.failOp = ToVkStencilOperation(depthStencilDesc.backStencilZFailOp);
//     stencilBack.reference = 0; //Dynamic
//     stencilBack.compareMask = depthStencilDesc.stencilReadMask;
//     stencilBack.writeMask = depthStencilDesc.stencilWriteMask;
//     depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
//     depthStencilInfo.pNext = nullptr;
//     depthStencilInfo.flags = 0;
//     depthStencilInfo.depthBoundsTestEnable = false;
//     depthStencilInfo.minDepthBounds = 0.0f;
//     depthStencilInfo.maxDepthBounds = 1.0f;
//     depthStencilInfo.depthTestEnable = depthStencilDesc.depthReadEnabled;
//     depthStencilInfo.depthWriteEnable = depthStencilDesc.depthWriteEnabled;
//     depthStencilInfo.depthCompareOp = ToVkCompareOperation(depthStencilDesc.depthCompareOp);
//     depthStencilInfo.front = stencilFront;
//     depthStencilInfo.back = stencilBack;
//     depthStencilInfo.stencilTestEnable = depthStencilDesc.stencilEnabled;

//     blendInfo = {};
//     const auto &blendDesc = blendState->GetDesc();
//     for(int32 i = 0; i < COLOR_ATTCHMENT_MAX_NUM; ++i)
//     {
//         auto &attach = blendDesc.attachments[i];
//         auto &blendAttachment = colorBlendAttachments[i];
//         blendAttachment.colorWriteMask = attach.writeMask & (VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
//         blendAttachment.blendEnable = attach.enabled;
//         blendAttachment.srcColorBlendFactor = ToVkBlendFactor(attach.srcFactor);
//         blendAttachment.dstColorBlendFactor = ToVkBlendFactor(attach.dstFactor);
//         blendAttachment.colorBlendOp = ToVkBlendOperation(attach.blendOp);
//         blendAttachment.srcAlphaBlendFactor = ToVkBlendFactor(attach.srcAlphaFactor);
//         blendAttachment.dstAlphaBlendFactor = ToVkBlendFactor(attach.dstAlphaFactor);
//         blendAttachment.alphaBlendOp = ToVkBlendOperation(attach.alphaBlendOp);
//     }
//     blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//     blendInfo.logicOpEnable = VK_FALSE;
//     blendInfo.logicOp = VK_LOGIC_OP_NO_OP;
//     blendInfo.attachmentCount = 1; //Dynamic
//     blendInfo.pAttachments = colorBlendAttachments;
//     blendInfo.blendConstants[0] = 0.0f;
//     blendInfo.blendConstants[1] = 0.0f;
//     blendInfo.blendConstants[2] = 0.0f;
//     blendInfo.blendConstants[3] = 0.0f;

//     dynamicInfo = {};
//     static auto dynamicStates = {
//          VK_DYNAMIC_STATE_VIEWPORT,
//          VK_DYNAMIC_STATE_SCISSOR,
//          VK_DYNAMIC_STATE_STENCIL_REFERENCE
//     };
//     dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//     dynamicInfo.pNext = nullptr;
//     dynamicInfo.flags = 0;
//     dynamicInfo.dynamicStateCount = dynamicStates.size();//3;
//     dynamicInfo.pDynamicStates = dynamicStates.begin();
// }

// SPtr<VulkanRenderPipeline> VulkanRenderPipeline::Create(const VulkanRenderPipelineCreateParams &params)
// {
//     return Memory::MakeShared<VulkanRenderPipeline>(params);
// }

// VulkanRenderPipeline::VulkanRenderPipeline(const VulkanRenderPipelineCreateParams &params)
// {
//     auto &context = VulkanContext::Get();
//     auto device = context.GetLogicalDeviceHandle();
//     VulkanRenderPipelineState *state = params.state.get();
//     VulkanVertexLayout *vertexLayout = params.vertexLayout.get();

//     //TEMP
//     VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
//     pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//     pipelineLayoutInfo.pNext = nullptr;
//     pipelineLayoutInfo.flags = 0;
//     pipelineLayoutInfo.setLayoutCount = 0;
//     pipelineLayoutInfo.pushConstantRangeCount = 0;
//     if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, gVulkanAlloc, &pipelineLayout) != VK_SUCCESS)
//         CT_EXCEPTION(RenderCore, "Create pipeline layout failed");

//     uint32 shaderStageCount = 2;
//     VkPipelineShaderStageCreateInfo shaderStages[] = {state->vertexShaderStageInfo, state->fragmentShaderStageInfo};

//     VkGraphicsPipelineCreateInfo pipelineInfo = {};
//     pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//     pipelineInfo.pNext = nullptr;
//     pipelineInfo.flags = 0;
//     pipelineInfo.stageCount = shaderStageCount;
//     pipelineInfo.pStages = shaderStages;
//     //pipelineInfo.pVertexInputState = &vertexLayout->createInfo;
//     pipelineInfo.pInputAssemblyState = &state->inputAssemblyInfo;
//     pipelineInfo.pViewportState = &state->viewportInfo;
//     pipelineInfo.pRasterizationState = &state->rasterizationInfo;
//     pipelineInfo.pMultisampleState = &state->multisampleInfo;
//     pipelineInfo.pColorBlendState = &state->blendInfo;
//     pipelineInfo.pDepthStencilState = &state->depthStencilInfo;
//     pipelineInfo.pDynamicState = &state->dynamicInfo;
//     pipelineInfo.layout = pipelineLayout;
//     pipelineInfo.renderPass = params.renderPass->GetHandle();
//     pipelineInfo.subpass = 0;                         //subpass index
//     pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; //TODO
//     pipelineInfo.basePipelineIndex = -1;              //TODO

//     if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, gVulkanAlloc, &pipeline) != VK_SUCCESS)
//         CT_EXCEPTION(RenderCore, "Create render pipeline failed.");
// }

// void VulkanRenderPipeline::Destroy()
// {
//     auto device = VulkanContext::Get().GetLogicalDeviceHandle();

//     if (pipeline != VK_NULL_HANDLE)
//     {
//         vkDestroyPipeline(device, pipeline, gVulkanAlloc);
//         pipeline = VK_NULL_HANDLE;
//     }

//     //TEMP
//     if (pipelineLayout != VK_NULL_HANDLE)
//     {
//         vkDestroyPipelineLayout(device, pipelineLayout, gVulkanAlloc);
//         pipelineLayout = VK_NULL_HANDLE;
//     }
// }

// }
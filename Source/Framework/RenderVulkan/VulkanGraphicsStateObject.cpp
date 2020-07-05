#include "RenderVulkan/VulkanGraphicsStateObject.h"
#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/VulkanRootSignature.h"
#include "RenderVulkan/VulkanProgram.h"

SPtr<GraphicsStateObject> GraphicsStateObject::Create(const GraphicsStateObjectDesc &desc)
{
    return Memory::MakeShared<VulkanGraphicsStateObject>(desc);
}

VulkanGraphicsStateObject::VulkanGraphicsStateObject(const GraphicsStateObjectDesc &desc)
    : GraphicsStateObject(desc)
{
    auto vkProgramKernel = static_cast<VulkanProgramKernel *>(desc.programKernel.get());
    Array<VkPipelineShaderStageCreateInfo> shaderInfos;
    for(int32 i = 0; i < (int32)ShaderType::Count; ++i)
    {
        ShaderType shaderType = (ShaderType)i;
        auto handle = vkProgramKernel->GetShaderModuleHandle(shaderType);
        if(handle)
        {
            VkPipelineShaderStageCreateInfo stageInfo = {};
            stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageInfo.stage = ToVkShaderStage(shaderType);
            stageInfo.module = handle;
            stageInfo.pName = "main";
            stageInfo.pSpecializationInfo = nullptr;

            shaderInfos.Add(stageInfo);
        }
    }
  
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    Array<VkVertexInputBindingDescription> vertexInputBindingDescs;
    Array<VkVertexInputAttributeDescription> vertexInputAttributeDescs;
    {
        const auto &bufferLayouts = desc.vertexLayout->GetBufferLayouts();
        for (int32 i = 0; i < bufferLayouts.Count(); ++i)
        {
            VkVertexInputBindingDescription bindingDesc = {};
            bindingDesc.binding = i;
            bindingDesc.stride = bufferLayouts[i]->GetStride();
            bindingDesc.inputRate = bufferLayouts[i]->IsPerInstanceData() ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
            vertexInputBindingDescs.Add(bindingDesc);

            int32 location = 0;
            for (const auto &e : bufferLayouts[i]->GetElements())
            {
                VkVertexInputAttributeDescription attrib = {};
                attrib.location = location++;
                attrib.format = ToVkResourceFormat(e.format);
                attrib.offset = e.offset;
                attrib.binding = i;
                for(uint32 i = 0; i < e.arrayLength; ++i)
                {
                    vertexInputAttributeDescs.Add(attrib);
                    attrib.offset += GetResourceFormatBytes(e.format);
                }
            }
        }

        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.pVertexBindingDescriptions = vertexInputBindingDescs.GetData();
        vertexInputInfo.vertexBindingDescriptionCount = vertexInputBindingDescs.Count();
        vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributeDescs.GetData();
        vertexInputInfo.vertexAttributeDescriptionCount = vertexInputAttributeDescs.Count();
    }

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
    {
        inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyInfo.topology = ToVkTopology(desc.topology);
        inputAssemblyInfo.primitiveRestartEnable = (desc.topology == Topology::TriangleStrip);
    }

    VkPipelineViewportStateCreateInfo viewportInfo = {};
    {
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = VIEWPORT_MAX_NUM;
        viewportInfo.pViewports = nullptr; //Dynamic
        viewportInfo.scissorCount = VIEWPORT_MAX_NUM;
        viewportInfo.pScissors = nullptr;  //Dynamic
    }

    VkPipelineRasterizationStateCreateInfo rasterizationInfo = {};
    {
        const auto &rasterizationDesc = desc.rasterizationState->GetDesc();
        rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationInfo.depthClampEnable = rasterizationDesc.depthClampEnabled;
        rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizationInfo.polygonMode = ToVkPolygonMode(rasterizationDesc.polygonMode);
        rasterizationInfo.lineWidth = 1.0f;
        rasterizationInfo.cullMode = ToVkCullMode(rasterizationDesc.cullMode);
        rasterizationInfo.frontFace = rasterizationDesc.frontCCW ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
        rasterizationInfo.depthBiasEnable = rasterizationDesc.depthBiasEnabled;
        rasterizationInfo.depthBiasConstantFactor = rasterizationDesc.depthBias;
        rasterizationInfo.depthBiasSlopeFactor = rasterizationDesc.slopeScaledDepthBias;
        rasterizationInfo.depthBiasClamp = rasterizationDesc.depthBiasClamp;
    }

    VkPipelineMultisampleStateCreateInfo multisampleInfo = {};
    {
        multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleInfo.sampleShadingEnable = VK_FALSE;
        multisampleInfo.rasterizationSamples = ToVkSampleCount(desc.frameBufferDesc.sampleCount);
        multisampleInfo.minSampleShading = 1.0f;
        multisampleInfo.pSampleMask = &desc.sampleMask;
        multisampleInfo.alphaToCoverageEnable = desc.blendState->GetDesc().alphaToCoverageEnabled;
        multisampleInfo.alphaToOneEnable = VK_FALSE;
    }

    VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
    {
        const auto &depthStencilDesc = desc.depthStencilState->GetDesc();
        VkStencilOpState stencilFront = {};
        stencilFront.compareOp = ToVkCompareOperation(depthStencilDesc.frontStencilCompareOp);
        stencilFront.depthFailOp = ToVkStencilOperation(depthStencilDesc.frontStencilZFailOp);
        stencilFront.passOp = ToVkStencilOperation(depthStencilDesc.frontStencilZFailOp);
        stencilFront.failOp = ToVkStencilOperation(depthStencilDesc.frontStencilZFailOp);
        stencilFront.reference = depthStencilDesc.stencilRef; //Dynamic
        stencilFront.compareMask = depthStencilDesc.stencilReadMask;
        stencilFront.writeMask = depthStencilDesc.stencilWriteMask;
        VkStencilOpState stencilBack = {};
        stencilBack.compareOp = ToVkCompareOperation(depthStencilDesc.backStencilCompareOp);
        stencilBack.depthFailOp = ToVkStencilOperation(depthStencilDesc.backStencilZFailOp);
        stencilBack.passOp = ToVkStencilOperation(depthStencilDesc.backStencilZFailOp);
        stencilBack.failOp = ToVkStencilOperation(depthStencilDesc.backStencilZFailOp);
        stencilBack.reference = depthStencilDesc.stencilRef; //Dynamic
        stencilBack.compareMask = depthStencilDesc.stencilReadMask;
        stencilBack.writeMask = depthStencilDesc.stencilWriteMask;

        depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        depthStencilInfo.minDepthBounds = 0.0f;
        depthStencilInfo.maxDepthBounds = 1.0f;
        depthStencilInfo.depthTestEnable = depthStencilDesc.depthReadEnabled;
        depthStencilInfo.depthWriteEnable = depthStencilDesc.depthWriteEnabled;
        depthStencilInfo.depthCompareOp = ToVkCompareOperation(depthStencilDesc.depthCompareOp);
        depthStencilInfo.front = stencilFront;
        depthStencilInfo.back = stencilBack;
        depthStencilInfo.stencilTestEnable = depthStencilDesc.stencilEnabled;
    }

    VkPipelineColorBlendStateCreateInfo blendInfo = {};
    Array<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
    {
        const auto &blendDesc = desc.blendState->GetDesc();
        for (int32 i = 0; i < blendDesc.attachments.Count(); ++i)
        {
            auto &attach = blendDesc.attachments[i];

            VkPipelineColorBlendAttachmentState blendAttachment = {};
            blendAttachment.colorWriteMask = attach.writeMask & (VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            blendAttachment.blendEnable = attach.enabled;
            blendAttachment.srcColorBlendFactor = ToVkBlendFactor(attach.srcFactor);
            blendAttachment.dstColorBlendFactor = ToVkBlendFactor(attach.dstFactor);
            blendAttachment.colorBlendOp = ToVkBlendOperation(attach.blendOp);
            blendAttachment.srcAlphaBlendFactor = ToVkBlendFactor(attach.srcAlphaFactor);
            blendAttachment.dstAlphaBlendFactor = ToVkBlendFactor(attach.dstAlphaFactor);
            blendAttachment.alphaBlendOp = ToVkBlendOperation(attach.alphaBlendOp);

            colorBlendAttachments.Add(blendAttachment);
        }
        blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        blendInfo.logicOpEnable = VK_FALSE;
        blendInfo.logicOp = VK_LOGIC_OP_NO_OP;
        blendInfo.attachmentCount = colorBlendAttachments.Count();
        blendInfo.pAttachments = colorBlendAttachments.GetData();
        blendInfo.blendConstants[0] = blendDesc.blendFactor[0];
        blendInfo.blendConstants[1] = blendDesc.blendFactor[1];
        blendInfo.blendConstants[2] = blendDesc.blendFactor[2];
        blendInfo.blendConstants[3] = blendDesc.blendFactor[3];
    }

    VkPipelineDynamicStateCreateInfo dynamicInfo = {};
    {
        static auto dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
            VK_DYNAMIC_STATE_STENCIL_REFERENCE};
        dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicInfo.dynamicStateCount = dynamicStates.size();
        dynamicInfo.pDynamicStates = dynamicStates.begin();
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderInfos.Count();
    pipelineInfo.pStages = shaderInfos.GetData();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportInfo;
    pipelineInfo.pRasterizationState = &rasterizationInfo;
    pipelineInfo.pMultisampleState = &multisampleInfo;
    pipelineInfo.pDepthStencilState = &depthStencilInfo;
    pipelineInfo.pColorBlendState = &blendInfo;
    pipelineInfo.pDynamicState = &dynamicInfo;
    pipelineInfo.layout = std::static_pointer_cast<VulkanRootSignature>(desc.rootSignature)->GetHandle();
    pipelineInfo.renderPass = (VkRenderPass)desc.frameBufferDesc.renderPass;
    pipelineInfo.subpass = 0; //subpass index
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(gVulkanDevice->GetLogicalDeviceHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, gVulkanAlloc, &pipeline) != VK_SUCCESS)
        CT_EXCEPTION(RenderCore, "Create render pipeline failed.");
}

VulkanGraphicsStateObject::~VulkanGraphicsStateObject()
{
    if (pipeline != VK_NULL_HANDLE)
    {
        if(gVulkanDevice)
        {
            gVulkanDevice->Release([pipeline = pipeline]() {
                vkDestroyPipeline(gVulkanDevice->GetLogicalDeviceHandle(), pipeline, gVulkanAlloc);
            });
        }
        pipeline = VK_NULL_HANDLE;
    }
}
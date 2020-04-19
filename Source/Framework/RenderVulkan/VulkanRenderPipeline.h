#pragma once

#include "RenderCore/RenderPipelineState.h"
#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

class VulkanRenderPipelineState : public RenderPipelineState
{
public:
    VulkanRenderPipelineState(const RenderPipelineStateCreateParams &params);

private:
    friend class VulkanRenderPipeline;

    SPtr<VulkanShader> shader;
    VkPipelineColorBlendAttachmentState colorBlendAttachments[COLOR_ATTCHMENT_MAX_NUM];

    VkPipelineShaderStageCreateInfo vertexShaderStageInfo;
    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineColorBlendStateCreateInfo blendInfo;
    VkPipelineDynamicStateCreateInfo dynamicInfo;
};

struct VulkanRenderPipelineCreateParams
{
    SPtr<VulkanRenderPipelineState> state;
    SPtr<VulkanRenderPass> renderPass;
};

class VulkanRenderPipeline : public IVulkanResource
{
public:
    VulkanRenderPipeline(const VulkanRenderPipelineCreateParams &params);

    virtual void Destroy() override;

    VkPipeline GetHandle() const
    {
        return pipeline;
    }

    bool IsMatch(const SPtr<VulkanRenderPass> &pass) const
    {
        return renderPass == pass;
    }

    static SPtr<VulkanRenderPipeline> Create(const VulkanRenderPipelineCreateParams &params);

private: 
    VkPipeline pipeline = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    SPtr<VulkanRenderPass> renderPass;
};
}
#pragma once

#include "RenderCore/Shader.h"
#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{
class VulkanShaderCompiler
{
public:
    void Init();
    void Destroy();

    Array<uchar8> Compile(ShaderType shaderType, const String &source);

    static VulkanShaderCompiler &Get()
    {
        static VulkanShaderCompiler instance;
        return instance;
    }
};

class VulkanShader : public Shader, public IVulkanResource
{
public:
    VulkanShader(const ShaderCreateParams &params);
    ~VulkanShader();
    virtual void Destroy() override;

    VkShaderModule GetVertexModuleHandle() const
    {
        return vertexModule;
    }

    VkShaderModule GetFragmentModuleHandle() const
    {
        return fragmentModule;
    }

private:
    VkShaderModule CreateShaderModule(const Array<uchar8> &code);

private:
    VkShaderModule vertexModule = VK_NULL_HANDLE;
    VkShaderModule fragmentModule = VK_NULL_HANDLE;
};
}
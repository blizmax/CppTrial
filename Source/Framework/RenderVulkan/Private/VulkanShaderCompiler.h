#pragma once

#include "RenderVulkan/.Package.h"
#include "RenderCore/ProgramReflection.h"

namespace RenderCore
{
class VulkanShaderCompiler
{
public:
    ~VulkanShaderCompiler() = default;

    virtual void Init() = 0;
    virtual void Deinit() = 0;
    virtual Array<uchar8> Compile(ShaderType shaderType, const String &source, const SPtr<ProgramReflection> &reflection) = 0;
};

class VulkanShaderCompilerImpl : public VulkanShaderCompiler
{
public:
    virtual void Init() override;
    virtual void Deinit() override;
    virtual Array<uchar8> Compile(ShaderType shaderType, const String &source, const SPtr<ProgramReflection> &reflection) override;
};

extern "C"
{
    CT_PRIVATE_API VulkanShaderCompiler *CreateVulkanShaderCompiler(void);
    CT_PRIVATE_API void DestroyVulkanShaderCompiler(VulkanShaderCompiler *);
    typedef VulkanShaderCompiler *(*CreateVulkanShaderCompilerFunc)(void);
    typedef void (*DestroyVulkanShaderCompilerFunc)(VulkanShaderCompiler *);
}

}
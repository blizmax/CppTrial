#pragma once

#include "RenderVulkan/.Package.h"
#include "RenderCore/Program.h"

namespace RenderCore
{
class VulkanShaderCompiler
{
public:
    using ShaderModuleFunc = std::function<void(ShaderType, const Array<uchar8>&)>;

    ~VulkanShaderCompiler() = default;

    virtual void Init() = 0;
    virtual void Deinit() = 0;
    virtual bool Compile(const ProgramDesc &desc, const ShaderModuleFunc &func, const ProgramReflectionBuilder &builder) = 0;
};

class VulkanShaderCompilerImpl : public VulkanShaderCompiler
{
public:
    virtual void Init() override;
    virtual void Deinit() override;
    virtual bool Compile(const ProgramDesc &desc, const ShaderModuleFunc &func, const ProgramReflectionBuilder &builder) override;
};

extern "C"
{
    CT_PRIVATE_API VulkanShaderCompiler *CreateVulkanShaderCompiler(void);
    CT_PRIVATE_API void DestroyVulkanShaderCompiler(VulkanShaderCompiler *);
    typedef VulkanShaderCompiler *(*CreateVulkanShaderCompilerFunc)(void);
    typedef void (*DestroyVulkanShaderCompilerFunc)(VulkanShaderCompiler *);
}

}
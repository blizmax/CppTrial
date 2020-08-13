#pragma once

#include "RenderCore/ProgramReflection.h"
#include "RenderCore/RootSignature.h"

using ProgramDefines = HashMap<String, String>;

struct ShaderDesc
{
    ShaderType shaderType;
    String source;
};

struct ProgramCompileOptions
{
    bool reflectAllVariables = false;
    bool generateDebugInfo = false;
};

struct ProgramDesc
{
    Array<ShaderDesc> shaderDescs;
    ProgramDefines defines;
    ProgramCompileOptions options;

    ProgramDesc() = default;
    ProgramDesc(std::initializer_list<ShaderDesc> initList)
        : shaderDescs(initList)
    {
    }

    bool HasShader(ShaderType shaderType) const
    {
        for (auto &d : shaderDescs)
        {
            if (d.shaderType == shaderType)
                return true;
        }
        return false;
    }
};

class ProgramKernel
{
public:
    ProgramKernel(const ProgramDesc &desc)
        : desc(desc)
    {
    }

    virtual ~ProgramKernel() = default;

    const SPtr<RootSignature> &GetRootSignature() const
    {
        return rootSignature;
    }

    const ProgramDesc &GetDesc() const
    {
        return desc;
    }

    const SPtr<ProgramReflection> &GetReflection() const
    {
        return reflection;
    }

    static SPtr<ProgramKernel> Create(const ProgramDesc &desc);

protected:
    ProgramDesc desc;
    SPtr<RootSignature> rootSignature;
    SPtr<ProgramReflection> reflection;
};

class Program
{
public:
    const ProgramDesc &GetDesc() const
    {
        return kernel->GetDesc();
    }

    const SPtr<ProgramReflection> &GetReflection() const
    {
        return kernel->GetReflection();
    }

    const SPtr<ProgramKernel> &GetKernel() const
    {
        return kernel;
    }

    static void SetCompileOptions(const ProgramCompileOptions &options)
    {
        sOptions = options;
    }

    static SPtr<Program> Create(const ProgramDesc &desc)
    {
        auto ptr = Memory::MakeShared<Program>();
        ptr->kernel = ProgramKernel::Create(desc);
        return ptr;
    }

    static SPtr<Program> Create(const String &path, const ProgramDefines &defines = {})
    {
        return Create(CreateDesc(path, defines));
    }

    /** API dependent */
    static ProgramDesc CreateDesc(const String &path, const ProgramDefines &defines = {});

private:
    SPtr<ProgramKernel> kernel;

    static ProgramCompileOptions sOptions;
};

inline ProgramCompileOptions Program::sOptions;
#pragma once

#include "RenderCore/RootSignature.h"
#include "RenderCore/ProgramReflection.h"

using ProgramDefines = HashMap<String, String>;

struct ShaderDesc
{
    ShaderType shaderType;
    String source;
};

struct ProgramCompileOptions
{
    bool printReflectionInfo = false;

    ProgramCompileOptions() = default;
};

struct ProgramDesc
{
    Array<ShaderDesc> shaderDescs;
    ProgramDefines defines;
    ProgramCompileOptions options;

    ProgramDesc() = default;
    ProgramDesc(std::initializer_list<ShaderDesc> initList) : shaderDescs(initList) {}
};

class ProgramKernel
{
public:
    ProgramKernel(const ProgramDesc &desc) : desc(desc)
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

    static SPtr<Program> Create(const ProgramDesc &desc)
    {
        auto ptr = Memory::MakeShared<Program>();
        ptr->kernel = ProgramKernel::Create(desc);
        return ptr;
    }

    static SPtr<Program> Create(const String &path, const ProgramDefines &defines = {}, const ProgramCompileOptions &options = {});

private:
    SPtr<ProgramKernel> kernel;
};
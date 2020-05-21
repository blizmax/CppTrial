#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

struct ShaderDesc
{
    ShaderType shaderType;
    String source;
};

struct ProgramDesc
{
    ProgramDesc() = default;
    ProgramDesc(std::initializer_list<ShaderDesc> initList) : shaderDescs(initList) {}

    Array<ShaderDesc> shaderDescs;
};

class Program
{
public:
    virtual ~Program() = default;

    static SPtr<Program> Create(const ProgramDesc &desc);

    const ProgramDesc &GetDesc() const
    {
        return desc;
    }

protected:
    Program(const ProgramDesc &desc) : desc(desc)
    {
    }

private:
    ProgramDesc desc;

};


}
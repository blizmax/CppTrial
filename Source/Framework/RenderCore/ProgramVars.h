#pragma once

#include "RenderCore/ParameterBlock.h"

namespace RenderCore
{

class CopyContext;
class ComputeContext;
class RenderContext;
class Program;

class ProgramVars : public ParameterBlock
{
public:
    virtual ~ProgramVars() = default;
};

class GraphicsVars : public ProgramVars
{
public:
    bool Apply(RenderContext *ctx, RootSignature *rootSignature)
    {
        return true;
    }

    static SPtr<GraphicsVars> Create(const SPtr<Program> &program);
};

class ComputeVars : public ProgramVars
{
public:
    //bool Apply(ComputeContext *ctx, RootSignature *rootSignature);
};

}
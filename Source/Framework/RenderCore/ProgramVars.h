#pragma once

#include "RenderCore/ParameterBlock.h"

class ProgramVars
{
public:
    ProgramVars(const SPtr<ProgramReflection> &reflection);
    virtual ~ProgramVars() = default;

    ShaderVar Root() const
    {
        return parameterBlock->GetRootVar();
    }

    void SetParameterBlock(const SPtr<ParameterBlock> &block)
    {
        CT_CHECK(block);
        parameterBlock = block;
    }

    const SPtr<ParameterBlock> &GetParameterBlock() const
    {
        return parameterBlock;
    }

protected:
    SPtr<ParameterBlock> parameterBlock;
    SPtr<ProgramReflection> reflection;
};

class GraphicsVars : public ProgramVars
{
public:
    GraphicsVars(const SPtr<ProgramReflection> &reflection);

    bool Apply(RenderContext *ctx, RootSignature *rootSignature);

    static SPtr<GraphicsVars> Create(const SPtr<Program> &program);
};

class ComputeVars : public ProgramVars
{
public:
    //bool Apply(ComputeContext *ctx, RootSignature *rootSignature);
};
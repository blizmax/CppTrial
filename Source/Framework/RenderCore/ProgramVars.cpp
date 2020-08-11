#include "RenderCore/ProgramVars.h"
#include "RenderCore/RenderContext.h"

ProgramVars::ProgramVars(const SPtr<ProgramReflection> &reflection)
    : reflection(reflection)
{
    parameterBlock = ParameterBlock::Create(reflection->GetDefaultBlockReflection());
}

SPtr<GraphicsVars> GraphicsVars::Create(const SPtr<Program> &program)
{
    return Memory::MakeShared<GraphicsVars>(program->GetReflection());
}

GraphicsVars::GraphicsVars(const SPtr<ProgramReflection> &reflection)
    : ProgramVars(reflection)
{
}

bool GraphicsVars::Apply(RenderContext *ctx, RootSignature *rootSignature)
{
    if (!parameterBlock->PrepareDescriptorSets(ctx))
        return false;

    auto &sets = parameterBlock->GetDescriptorSets();
    for (int32 i = 0; i < sets.Count(); ++i)
    {
        if (sets[i])
            sets[i]->BindForGraphics(ctx, rootSignature, i);
    }

    return true;
}

SPtr<ComputeVars> ComputeVars::Create(const SPtr<Program> &program)
{
    return Memory::MakeShared<ComputeVars>(program->GetReflection());
}

ComputeVars::ComputeVars(const SPtr<ProgramReflection> &reflection)
    : ProgramVars(reflection)
{
}

bool ComputeVars::Apply(ComputeContext *ctx, RootSignature *rootSignature)
{
    if (!parameterBlock->PrepareDescriptorSets(ctx))
        return false;

    auto &sets = parameterBlock->GetDescriptorSets();
    for (int32 i = 0; i < sets.Count(); ++i)
    {
        if (sets[i])
            sets[i]->BindForCompute(ctx, rootSignature, i);
    }

    return true;
}
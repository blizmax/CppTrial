#pragma once

#include "RenderCore/ComputeState.h"
#include "RenderCore/ProgramVars.h"

class ComputePass
{
public:
    ComputePass(const ProgramDesc &desc);

    virtual ~ComputePass() = default;

    virtual void Execute(ComputeContext *ctx, int32 threadsX, int32 threadsY, int32 threadsZ = 1);
    virtual void Execute(ComputeContext *ctx, const Vector3I threads);
    virtual void ExecuteIndirect(ComputeContext *ctx, const Buffer *argBuffer, uint64 argBufferOffset = 0);
    Vector3I GetThreadGroupSize() const;

    const SPtr<ComputeVars> &GetVars() const
    {
        return vars;
    }

    ShaderVar GetRootVar() const
    {
        return vars->Root();
    }

    const SPtr<ComputeState> &GetState() const
    {
        return state;
    }

    const SPtr<Program> &GetProgram() const
    {
        return state->GetProgram();
    }

    static SPtr<ComputePass> Create(const ProgramDesc &desc);
    static SPtr<ComputePass> Create(const String &path, const ProgramDefines &defines = {});

protected:
    SPtr<ComputeVars> vars;
    SPtr<ComputeState> state;
};
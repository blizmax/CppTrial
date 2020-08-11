#pragma once

#include "RenderCore/ComputeStateObject.h"
#include "RenderCore/Program.h"
#include "RenderCore/ProgramVars.h"
#include "RenderCore/StateGraph.h"

class ComputeState
{
public:
    ComputeState();
    ~ComputeState() = default;

    SPtr<ComputeStateObject> GetCso(const ComputeVars *vars);

    void SetProgram(const SPtr<Program> &prog)
    {
        program = prog;
    }

    const SPtr<Program> &GetProgram() const
    {
        return program;
    }

    const ComputeStateObjectDesc &GetDesc() const
    {
        return desc;
    }

    static SPtr<ComputeState> Create();

private:
    struct CachedData
    {
        WPtr<ProgramKernel> programKernel;
        WPtr<RootSignature> rootSignature;
    };

    SPtr<Program> program;
    ComputeStateObjectDesc desc;

    CachedData cachedData;
    StateGraph<SPtr<ComputeStateObject>> stateGraph;
};
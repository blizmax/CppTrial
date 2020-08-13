#pragma once

#include "RenderCore/GraphicsState.h"
#include "RenderCore/ProgramVars.h"

class GraphicsPass
{
public:
    GraphicsPass(const ProgramDesc &desc)
    {
        auto program = Program::Create(desc);
        state = GraphicsState::Create();
        state->SetProgram(program);
        vars = GraphicsVars::Create(program);
    }

    virtual ~GraphicsPass() = default;

    const SPtr<GraphicsVars> &GetVars() const
    {
        return vars;
    }

    ShaderVar GetRootVar() const
    {
        return vars->Root();
    }

    const SPtr<GraphicsState> &GetState() const
    {
        return state;
    }

    const SPtr<Program> &GetProgram() const
    {
        return state->GetProgram();
    }

protected:
    SPtr<GraphicsVars> vars;
    SPtr<GraphicsState> state;
};
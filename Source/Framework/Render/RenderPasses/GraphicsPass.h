#pragma once

#include "Render/ProgramVars.h"
#include "Render/GraphicsState.h"

class GraphicsPass
{
public:
    virtual ~GraphicsPass() = default;

    const SPtr<GraphicsVars> &GetVars() const
    {
        return vars;
    }

    const SPtr<GraphicsState> &GetState() const
    {
        return state;
    }

protected:
    SPtr<GraphicsVars> vars;
    SPtr<GraphicsState> state;
};
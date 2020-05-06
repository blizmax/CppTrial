#pragma once

#include "RenderCore/Shader.h"
#include "RenderCore/RasterizerState.h"
#include "RenderCore/DepthStencilState.h"
#include "RenderCore/BlendState.h"

namespace RenderCore
{
struct StateObjectDesc
{
    SPtr<RasterizationState> rasterizationState;
    SPtr<DepthStencilState> depthStencilState;
    SPtr<BlendState> blendState;
};

class StateObject
{
public:
    StateObject(const StateObjectDesc &desc) : desc(desc)
    {
    }

    const StateObjectDesc &GetDesc() const
    {
        return desc;
    }

    SPtr<StateObject> Create(const StateObjectDesc &desc)
    {
        return Memory::MakeShared<StateObject>(desc);
    }
private:
    StateObjectDesc desc;
};
}
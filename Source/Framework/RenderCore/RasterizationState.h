#pragma once

#include "RenderCore/.Package.h"

struct RasterizationStateDesc
{
    PolygonMode polygonMode = PolygonMode::Fill;
    CullMode cullMode = CullMode::Back;
    bool scissorEnabled = true;
    bool depthClampEnabled = true;
    bool depthBiasEnabled = false;
    float depthBias = 0.0f;
    float depthBiasClamp = 0.0f;
    float slopeScaledDepthBias = 0.0f;
};

class RasterizationState
{
public:
    RasterizationState(const RasterizationStateDesc &desc) : desc(desc)
    {
    }

    const RasterizationStateDesc &GetDesc() const
    {
        return desc;
    }

    static SPtr<RasterizationState> Create(const RasterizationStateDesc &desc)
    {
        return Memory::MakeShared<RasterizationState>(desc);
    }

private:
    RasterizationStateDesc desc;
};
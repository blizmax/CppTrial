#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
struct RasterizationStateData
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

using RasterizationStateCreateParams = RasterizationStateData;

class RasterizationState
{
public:
    RasterizationState(const RasterizationStateCreateParams &params) : data(params)
    {
    }

    auto GetData() const
    {
        return data;
    }

    static SPtr<RasterizationState> Create(const RasterizationStateCreateParams &params)
    {
        return Memory::MakeShared<RasterizationState>(params);
    }

private:
    RasterizationStateData data;
};
}
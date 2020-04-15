#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
struct DepthStencilStateData
{
    bool depthReadEnabled = true;
    bool depthWriteEnabled = true;
    CompareOperation depthCompareOp = CompareOperation::Less;

    bool stencilEnabled = false;
    uint8 stencilReadMask = 0xFF;
    uint8 stencilWriteMask = 0xFF;
    CompareOperation frontStencilCompareOp = CompareOperation::AlwaysPass;
    StencilOperation frontStencilFailOp = StencilOperation::Keep;
    StencilOperation frontStencilZFailOp = StencilOperation::Keep;
    StencilOperation frontStencilPassOp = StencilOperation::Keep;
    CompareOperation backStencilCompareOp = CompareOperation::AlwaysPass;
    StencilOperation backStencilFailOp = StencilOperation::Keep;
    StencilOperation backStencilZFailOp = StencilOperation::Keep;
    StencilOperation backStencilPassOp = StencilOperation::Keep;
};

using DepthStencilStateCreateParams = DepthStencilStateData;

class DepthStencilState
{
public:
    DepthStencilState(const DepthStencilStateCreateParams &params) : data(params)
    {
    }

    auto GetData() const
    {
        return data;
    }

    static SPtr<DepthStencilState> Create(const DepthStencilStateCreateParams &params)
    {
        return Memory::MakeShared<DepthStencilState>(params);
    }

private:
    DepthStencilStateData data;
};
}
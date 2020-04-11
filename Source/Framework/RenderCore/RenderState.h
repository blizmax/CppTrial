#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
class BlendState
{
public:
    bool enabled = false;
    uint8 writeMask = 0xFF;
    BlendFactor srcFactor = BlendFactor::SrcAlpha;
    BlendFactor dstFactor = BlendFactor::OneMinusSrcAlpha;
    BlendOperation blendOp = BlendOperation::Add;

    static SPtr<BlendState> Create()
    {
        return Memory::MakeShared<BlendState>();
    }
};

class DepthStencilState
{
public:
    bool depthReadEnabled = true;
    bool depthWriteEnabled = true;
    CompareOperation depthComp = CompareOperation::Less;

    bool stencilEnabled = false;
    uint8 stencilReadMask = 0xFF;
    uint8 stencilWriteMask = 0xFF;
    CompareOperation stencilComp = CompareOperation::AlwaysPass;
    StencilOperation stencilFailOp = StencilOperation::Keep;
    StencilOperation stencilZFailOp = StencilOperation::Keep;
    StencilOperation stencilPassOp = StencilOperation::Keep;

    static SPtr<DepthStencilState> Create()
    {
        return Memory::MakeShared<DepthStencilState>();
    }
};
}
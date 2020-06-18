#pragma once

#include "RenderCore/.Package.h"

struct DepthStencilStateDesc
{
    bool depthReadEnabled = true;
    bool depthWriteEnabled = true;
    CompareOperation depthCompareOp = CompareOperation::Less;

    bool stencilEnabled = false;
    uint8 stencilReadMask = 0xFF;
    uint8 stencilWriteMask = 0xFF;
    uint8 stencilRef = 0;
    CompareOperation frontStencilCompareOp = CompareOperation::AlwaysPass;
    StencilOperation frontStencilFailOp = StencilOperation::Keep;
    StencilOperation frontStencilZFailOp = StencilOperation::Keep;
    StencilOperation frontStencilPassOp = StencilOperation::Keep;
    CompareOperation backStencilCompareOp = CompareOperation::AlwaysPass;
    StencilOperation backStencilFailOp = StencilOperation::Keep;
    StencilOperation backStencilZFailOp = StencilOperation::Keep;
    StencilOperation backStencilPassOp = StencilOperation::Keep;
};

class DepthStencilState
{
public:
    DepthStencilState(const DepthStencilStateDesc &desc) : desc(desc)
    {
    }

    const DepthStencilStateDesc &GetDesc() const
    {
        return desc;
    }

    static SPtr<DepthStencilState> Create(const DepthStencilStateDesc &desc)
    {
        return Memory::MakeShared<DepthStencilState>(desc);
    }

private:
    DepthStencilStateDesc desc;
};
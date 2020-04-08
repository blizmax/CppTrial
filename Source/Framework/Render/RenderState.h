#pragma once

#include "Render/.Package.h"

class BlendState
{
public:
    bool enabled = false;
    uint8 writeMask = 0xFF;
    BlendFactor srcFactor = BlendFactor::SrcAlpha;
    BlendFactor dstFactor = BlendFactor::OneMinusSrcAlpha;
    BlendOperation blendOp = BlendOperation::Add;
};

class DepthState
{
public:
    bool readEnabled = true;
    bool writeEnabled = true; 
    CompareOperation depthComp = CompareOperation::Less;
};

class StencilState
{
public:
    bool enabled = false;
    uint8 readMask = 0xFF;
    uint8 writeMask = 0xFF;
    CompareOperation stencilComp = CompareOperation::AlwaysPass;
    StencilOperation stencilFailOp = StencilOperation::Keep;
    StencilOperation depthFailOp = StencilOperation::Keep;
    StencilOperation passOp = StencilOperation::Keep;
};
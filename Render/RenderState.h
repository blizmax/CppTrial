#pragma once

#include "Render/.Package.h"

class BlendState
{
public:
    BlendFactor srcFactor = BlendFactor::SrcAlpha;
    BlendFactor dstFactor = BlendFactor::OneMinusSrcAlpha;
    BlendOperation operation = BlendOperation::Add;
    bool enabled = false;
};

class DepthState
{
public:
    bool readEnabled = true;
    bool writeEnabled = true;    
};
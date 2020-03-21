#pragma once

#include "Core/String.h"

class Page
{
public:    
    virtual String GetName() = 0;
    virtual String GetShaderPath() = 0;

    virtual void OnImGuiDraw() = 0;
};
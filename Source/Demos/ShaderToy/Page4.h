#pragma once

#include "Demos/ShaderToy/Page.h"

class Page4 : public TPage<Page4>
{
public:
    virtual String GetName() override
    {
        return CT_TEXT("Color Wheel(HSV)");
    }
};
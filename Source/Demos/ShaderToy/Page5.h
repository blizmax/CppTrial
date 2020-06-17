#pragma once

#include "Demos/ShaderToy/Page.h"

class Page5 : public TPage<Page5>
{
public:
    virtual String GetName() override
    {
        return CT_TEXT("Hello 3D");
    }
};
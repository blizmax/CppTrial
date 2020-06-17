#pragma once

#include "Demos/ShaderToy/Page.h"

class Page3 : public TPage<Page3>
{
public:
    virtual String GetName() override
    {
        return CT_TEXT("Shapes");
    }
};
#pragma once

#include "Demos/ShaderToy/Page.h"

class Page6 : public TPage<Page6>
{
public:
    virtual String GetName() override
    {
        return CT_TEXT("Noise");
    }
};
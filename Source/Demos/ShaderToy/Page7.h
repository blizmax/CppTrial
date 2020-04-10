#pragma once

#include "Demos/ShaderToy/Page.h"

class Page7 : public TPage<Page7>
{
public:
    virtual String GetName() override
    {
        return CT_TEXT("Cell");
    }

    virtual void OnImGuiDraw() override
    {
    }

    virtual void OnShaderUpdate(SPtr<Shader> &shader) override
    {
    }
};
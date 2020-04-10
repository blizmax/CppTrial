#pragma once

#include "Demos/ShaderToy/Page.h"

class Page1 : public TPage<Page1>
{
public:
    virtual String GetName() override
    {
        return CT_TEXT("Builtin Functions");
    }

    virtual void OnImGuiDraw() override
    {
        
    }

    virtual void OnShaderUpdate(SPtr<Shader> &shader) override
    {

    }
};
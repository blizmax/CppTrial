#pragma once

#include "Demo/Page.h"

class Page1 : public Page
{
public:
    virtual String GetName() override
    {
        return CT_TEXT("Builtin Functions");
    }

    virtual String GetShaderPath() override
    {
        return CT_TEXT("Assets/Shaders/Demos/Shader1.glsl");
    }

    virtual void OnImGuiDraw() override
    {
        
    }

    virtual void OnShaderUpdate(SPtr<Shader> &shader) override
    {

    }

    static UPtr<Page> Create()
    {
        return Memory::MakeUnique<Page1>();
    }
};
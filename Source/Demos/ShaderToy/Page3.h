#pragma once

#include "Demos/ShaderToy/Page.h"

class Page3 : public Page
{
public:
    virtual String GetName() override
    {
        return CT_TEXT("Shapes");
    }

    virtual void OnImGuiDraw() override
    {

    }

    virtual void OnShaderUpdate(SPtr<Shader> &shader) override
    {

    }

    static UPtr<Page> Create()
    {
        return Memory::MakeUnique<Page3>();
    }
};
#pragma once

#include "Demo/Page.h"

class Page4 : public Page
{
public:
    virtual String GetName() override
    {
        return CT_TEXT("Color Wheel(HSV)");
    }

    virtual void OnImGuiDraw() override
    {
    }

    virtual void OnShaderUpdate(SPtr<Shader> &shader) override
    {
    }

    static UPtr<Page> Create()
    {
        return Memory::MakeUnique<Page4>();
    }
};
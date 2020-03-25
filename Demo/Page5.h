#pragma once

#include "Demo/Page.h"

class Page5 : public Page
{
public:
    virtual String GetName() override
    {
        return CT_TEXT("Hello 3D");
    }

    virtual void OnImGuiDraw() override
    {
    }

    virtual void OnShaderUpdate(SPtr<Shader> &shader) override
    {
    }

    static UPtr<Page> Create()
    {
        return Memory::MakeUnique<Page5>();
    }
};
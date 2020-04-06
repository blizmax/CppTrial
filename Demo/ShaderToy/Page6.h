#pragma once

#include "Demo/ShaderToy/Page.h"

class Page6 : public Page
{
public:
    virtual String GetName() override
    {
        return CT_TEXT("Noise");
    }

    virtual void OnImGuiDraw() override
    {
    }

    virtual void OnShaderUpdate(SPtr<Shader> &shader) override
    {
    }

    static UPtr<Page> Create()
    {
        return Memory::MakeUnique<Page6>();
    }
};
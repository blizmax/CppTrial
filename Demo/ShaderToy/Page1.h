#pragma once

#include "Demo/ShaderToy/Page.h"

class Page1 : public Page
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

    static UPtr<Page> Create()
    {
        return Memory::MakeUnique<Page1>();
    }
};
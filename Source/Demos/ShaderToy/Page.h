#pragma once

#include "RenderCore/ProgramVars.h"

class Page
{
public:
    virtual String GetName() = 0;

    virtual void OnShaderUpdate(SPtr<GraphicsVars> &vars)
    {
    }

    virtual void OnImGuiDraw()
    {
    }

    static String GetShaderDirectory()
    {
        return CT_TEXT("Assets/Shaders/ShaderToy/");
    }
};

template <typename T>
class TPage : public Page
{
public:
    static UPtr<TPage> Create()
    {
        return Memory::MakeUnique<T>();
    }
};
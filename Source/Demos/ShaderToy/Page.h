#pragma once

#include "Render/Shader.h"

class Page
{
public:
    virtual String GetName() = 0;

    virtual void OnShaderUpdate(SPtr<Shader> &shader) = 0;
    virtual void OnImGuiDraw() = 0;

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
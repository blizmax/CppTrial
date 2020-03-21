#pragma once

#include "Render/Shader.h"

class Page
{
public:
    virtual String GetName() = 0;
    virtual String GetShaderPath() = 0;

    virtual void OnShaderUpdate(SPtr<Shader> &shader) = 0;
    virtual void OnImGuiDraw() = 0;
};
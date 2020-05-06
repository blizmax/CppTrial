#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
struct ShaderDesc
{
    String vertexSource;
    String fragmentSource;
};

class Shader
{
public:
    static SPtr<Shader> Create(const ShaderDesc &desc);
};
}
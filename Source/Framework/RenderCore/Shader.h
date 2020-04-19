#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
struct ShaderCreateParams
{
    String vertexSource;
    String fragmentSource;
};

class Shader
{
public:
    static SPtr<Shader> Create(const ShaderCreateParams &params);
};
}
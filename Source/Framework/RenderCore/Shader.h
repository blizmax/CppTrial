#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
struct ShaderCreateParams
{
    Array<uchar8> vertexSource;
    Array<uchar8> fragmentSource;
};

class Shader
{
public:
    static SPtr<Shader> Create(const ShaderCreateParams &params);
};
}
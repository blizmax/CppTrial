#pragma once

#include "Render/OpenGL/.Package.h"
#include "Render/Shader.h"

class GLShader : public Shader
{
public:
    GLShader(const String &path);
    virtual ~GLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
    struct ShaderSource
    {
        String vertex;
        String fragment;
    };

    ShaderSource Parse(const String &path);
    bool Compile(uint32 shaderID, const String &source);
    bool Link();

private:
    uint32 id = 0;
};
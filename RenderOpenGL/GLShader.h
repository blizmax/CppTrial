#pragma once

#include "RenderOpenGL/.Package.h"
#include "Render/Shader.h"
#include "Core/HashMap.h"

class GLShader : public Shader
{
public:
    GLShader(const String &path);
    virtual ~GLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetInt(const String &name, int32 value) override;
    virtual void SetFloat(const String &name, float value) override;
    virtual void SetVector2(const String &name, const Vector2 &value) override;
    virtual void SetVector3(const String &name, const Vector3 &value) override;
    virtual void SetVector4(const String &name, const Vector4 &valeu) override;
    virtual void SetMatrix4(const String &name, const Matrix4 &value) override;
    virtual void SetColor(const String &name, const Color &value) override;

private:
    struct ShaderSource
    {
        String vertex;
        String fragment;
    };

    ShaderSource Parse(const String &path);
    bool Compile(uint32 shaderID, const String &source);
    bool Link();

    int32 GetUniformLocation(const String& name);

private:
    uint32 id = 0;
    HashMap<String, int32> uniformLocations;
}; 
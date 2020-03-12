#pragma once

#include "Render/.Package.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4.h"

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const String &name, int32 value) = 0;
    virtual void SetFloat(const String &name, float value) = 0;
    virtual void SetVector3(const String &name, const Vector3 &value) = 0;
    virtual void SetVector4(const String &name, const Vector4 &valeu) = 0;
    virtual void SetMatrix4(const String &name, const Matrix4 &value) = 0;

    static SPtr<Shader> Create(const String &path);
};
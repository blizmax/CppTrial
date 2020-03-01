#pragma once

#include "Render/.Package.h"

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static SPtr<Shader> Create(const String &path);
};
#pragma once

#include "Render/.Package.h"

class RenderTexture
{
public:
    void Bind() const;
    void Unbind() const;

    virtual uint32 GetWidth() const = 0;
    virtual uint32 GetHeight() const = 0;

    static SPtr<RenderTexture> Create(uint32 width, uint32 height);
};
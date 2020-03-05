#pragma once

#include "Render/.Package.h"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual void SetData(void *data, uint32 size) = 0;
    virtual void Bind(uint32 slot = 0) const = 0;

    virtual uint32 GetWidth() const = 0;
    virtual uint32 GetHeight() const = 0;

    static SPtr<Texture> Create(uint32 width, uint32 height);
    static SPtr<Texture> Create(const String &path);
};
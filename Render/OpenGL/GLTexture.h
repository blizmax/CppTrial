#pragma once

#include "Render/OpenGL/.Package.h"
#include "Render/Texture.h"

class GLTexture : public Texture
{
public:
    GLTexture(uint32 width, uint32 height);
    GLTexture(const String &path);
    virtual ~GLTexture();

    virtual void SetData(void *data, uint32 size) override;
    virtual void Bind(uint32 slot) const override;

    virtual uint32 GetWidth() const override
    {
        return width;
    }

    virtual uint32 GetHeight() const override
    {
        return height;
    }

private:
    uint32 id = 0;
    uint32 width;
    uint32 height;
    uint32 dataFormat = 0;
    uint32 internalFormat = 0;
};
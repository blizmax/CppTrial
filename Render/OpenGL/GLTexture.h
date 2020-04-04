#pragma once

#include "Render/OpenGL/.Package.h"
#include "Render/Texture.h"

class GLTexture : public Texture
{
public:
    GLTexture(uint32 width, uint32 height);
    GLTexture(const String &path);
    GLTexture(const SPtr<Pixmap> &pixmap);
    virtual ~GLTexture();

    virtual void SetData(const void *data, uint32 size) override;
    virtual void Bind(int32 slot = -1) const override;

    virtual void SetMinFilter(TextureFilter filter) override;
    virtual void SetMagFilter(TextureFilter filter) override;
    virtual void SetMipFilter(TextureFilter filter) override;

    virtual void SetUWrap(TextureWrap wrap) override;
    virtual void SetVWrap(TextureWrap wrap) override;

    virtual uint32 GetWidth() const override
    {
        return width;
    }

    virtual uint32 GetHeight() const override
    {
        return height;
    }

    virtual TextureFilter GetMinFilter() const override
    {
        return minFilter;
    }

    virtual TextureFilter GetMagFilter() const override
    {
        return magFilter;
    }

    virtual TextureFilter GetMipFilter() const override
    {
        return mipFilter;
    }

    virtual TextureWrap GetUWrap() const override
    {
        return uWrap;
    }

    virtual TextureWrap GetVWrap() const override
    {
        return vWrap;
    }

    virtual void *GetNativeHandle() const override
    {
        return reinterpret_cast<void*>(id);
    }

private:
    uint32 id = 0;
    uint32 width;
    uint32 height;
    PixelFormat format = PixelFormat::RGBA8888;
    TextureFilter minFilter = TextureFilter::Nearest;
    TextureFilter magFilter = TextureFilter::Nearest;
    TextureFilter mipFilter = TextureFilter::Nearest;
    TextureWrap uWrap = TextureWrap::Repeat;
    TextureWrap vWrap = TextureWrap::Repeat;
    bool useMip = false;
};
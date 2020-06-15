#include "RenderOpenGL/GLTexture.h"
#include "Render/ImageLoader.h"

SPtr<Texture> Texture::Create(uint32 width, uint32 height)
{
    return Memory::MakeShared<GLTexture>(width, height);
}

SPtr<Texture> Texture::Create(const String &path)
{
    return Memory::MakeShared<GLTexture>(path);
}

SPtr<Texture> Texture::Create(const SPtr<Pixmap> &pixmap)
{
    return Memory::MakeShared<GLTexture>(pixmap);
}

GLTexture::GLTexture(uint32 width, uint32 height)
    : width(width), height(height)
{
    uint32 internalFormat = Render::GetGLPixelInternalFormat(format);

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    Bind();

    glTextureStorage2D(id, 1, internalFormat, width, height);

    SetMinFilter(minFilter);
    SetMagFilter(magFilter);
    SetUWrap(uWrap);
    SetVWrap(vWrap);
}

GLTexture::GLTexture(const String &path)
{
    int32 w, h, channels;
    void *data = ImageLoader::Load(path, w, h, channels, true);
    if (!data)
    {
        CT_EXCEPTION(Render, "Load texture data failed.");
    }

    if (channels == 4)
    {
        format = PixelFormat::RGBA8888;
    }
    else if (channels == 3)
    {
        format = PixelFormat::RGB888;
    }
    else
    {
        CT_EXCEPTION(Render, "Unsupported data format.");
    }

    width = w;
    height = h;

    uint32 internalFormat = Render::GetGLPixelInternalFormat(format);
    uint32 dataFormat = Render::GetGLPixelFormat(format);
    uint32 dataType = Render::GetGLPixelDataType(format);

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    Bind();

    glTextureStorage2D(id, 1, internalFormat, width, height);

    SetMinFilter(minFilter);
    SetMagFilter(magFilter);
    SetUWrap(uWrap);
    SetVWrap(vWrap);

    glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, dataType, data);

    ImageLoader::Free(data);
}

GLTexture::GLTexture(const SPtr<Pixmap> &pixmap)
    : width(pixmap->GetWidth()), height(pixmap->GetHeight()), format(pixmap->GetFormat())
{
    uint32 internalFormat = Render::GetGLPixelInternalFormat(format);

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    Bind();

    glTextureStorage2D(id, 1, internalFormat, width, height);

    SetMinFilter(minFilter);
    SetMagFilter(magFilter);
    SetUWrap(uWrap);
    SetVWrap(vWrap);

    SetData(pixmap->GetData(), pixmap->GetSize());
}

GLTexture::~GLTexture()
{
    glDeleteTextures(1, &id);
}

void GLTexture::SetData(const void *data, uint32 size)
{
    Bind();

    uint32 dataFormat = Render::GetGLPixelFormat(format);
    uint32 dataType = Render::GetGLPixelDataType(format);

    glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, dataType, data);
}

void GLTexture::Bind(int32 slot) const
{
    glBindTexture(GL_TEXTURE_2D, id);
    
    if (slot >= 0)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
    }
}

void GLTexture::SetMinFilter(TextureFilter filter)
{
    Bind();

    minFilter = filter;
    if (useMip)
    {
        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, Render::GetGLMinMipFilter(minFilter, mipFilter));
    }
    else
    {
        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, Render::GetGLTextureFilter(filter));
    }
}

void GLTexture::SetMagFilter(TextureFilter filter)
{
    Bind();

    magFilter = filter;

    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, Render::GetGLTextureFilter(filter));
}

void GLTexture::SetMipFilter(TextureFilter filter)
{
    Bind();

    mipFilter = filter;
    if (useMip)
    {
        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, Render::GetGLMinMipFilter(minFilter, mipFilter));
    }
}

void GLTexture::SetUWrap(TextureWrap wrap)
{
    Bind();

    uWrap = wrap;

    glTextureParameteri(id, GL_TEXTURE_WRAP_S, Render::GetGLTextureWrap(wrap));
}

void GLTexture::SetVWrap(TextureWrap wrap)
{
    Bind();

    vWrap = wrap;

    glTextureParameteri(id, GL_TEXTURE_WRAP_T, Render::GetGLTextureWrap(wrap));
}

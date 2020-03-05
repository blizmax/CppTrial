#include "Render/OpenGL/GLTexture.h"
#include "Render/ImageLoader.h"

SPtr<Texture> Texture::Create(uint32 width, uint32 height)
{
    return Memory::MakeShared<GLTexture>(width, height);
}

SPtr<Texture> Texture::Create(const String &path)
{
    return Memory::MakeShared<GLTexture>(path);
}

GLTexture::GLTexture(uint32 width, uint32 height) : width(width), height(height)
{
    internalFormat = GL_RGBA8;
    dataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    glTextureStorage2D(id, 1, internalFormat, width, height);

    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

GLTexture::GLTexture(const String &path)
{
    int32 channels;
    void *data = ImageLoader::Load(path, width, height, channels, true);
    if (!data)
    {
        CT_EXCEPTION(Render, "Load texture data failed.");
    }

    if (channels == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (channels == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }
    else
    {
        CT_EXCEPTION(Render, "Unsupported data format.");
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    glTextureStorage2D(id, 1, internalFormat, width, height);

    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

    ImageLoader::Free(data);
}

GLTexture::~GLTexture()
{
    glDeleteTextures(1, &id);
}

void GLTexture::SetData(void *data, uint32 size)
{
    uint32 bpp = dataFormat == GL_RGBA ? 4 : 3;
    if (bpp * width * height != size)
    {
        CT_EXCEPTION(Render, "Update texture data failed, size error.");
    }

    glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
}

void GLTexture::Bind(uint32 slot) const
{
    glBindTextureUnit(slot, id);
}
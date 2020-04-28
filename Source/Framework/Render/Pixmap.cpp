#include "Render/Pixmap.h"

SPtr<Pixmap> Pixmap::Create(int32 width, int32 height, PixelFormat format)
{
    return Memory::MakeShared<Pixmap>(width, height, format);
}

SPtr<Pixmap> Pixmap::Create(int32 width, int32 height, PixelFormat format, const void *src)
{
    return Memory::MakeShared<Pixmap>(width, height, format, src);
}

Pixmap::Pixmap(int32 width, int32 height, PixelFormat format)
    : width(width), height(height), format(format)
{
    CT_CHECK(width >= 0 && height >= 0);

    data = Memory::Alloc(GetSize());

    Fill(Color::WHITE);
}

Pixmap::Pixmap(int32 width, int32 height, PixelFormat format, const void *src)
    : width(width), height(height), format(format)
{
    CT_CHECK(width >= 0 && height >= 0);

    data = Memory::Alloc(GetSize());
    std::memcpy(data, src, GetSize());
}

Pixmap::~Pixmap()
{
    Memory::Free(data);
}

uint32 Pixmap::GetSize() const
{
    return width * height * Render::GetPixelSize(format);
}

void Pixmap::Fill(const Color &color)
{
    auto ptr = static_cast<uint8 *>(data);
    Render::PackColor(color, format, ptr, width * height);
}

void Pixmap::SetColor(int32 x, int32 y, const Color &color)
{
    if(!IsValid(x, y))
        return;

    auto ptr = static_cast<uint8 *>(data) + (x + width * y) * Render::GetPixelSize(format);
    Render::PackColor(color, format, ptr);
}

void Pixmap::SetColors(const Color *src, int32 count)
{
    const int32 max = width * height;
    count = count >= max ? max : count;
    auto ptr = static_cast<uint8 *>(data);
    auto bytesPerPixel = Render::GetPixelSize(format);

    for(int32 i = 0; i < count; ++i)
    {
        Render::PackColor(src[i], format, ptr);
        ptr += bytesPerPixel;
    }
}

void Pixmap::SetColors(const Array<Color> &colors)
{
    SetColors(colors.GetData(), colors.Count());
}

Color Pixmap::GetColor(int32 x, int32 y)
{
    Color result = {0.0f, 0.0f, 0.0f, 0.0f};

    if (!IsValid(x, y))
        return result;

    auto ptr = static_cast<uint8 *>(data);
    Render::UnpackColor(&result, format, ptr);
    return result;
}

Array<Color> Pixmap::GetColors() const
{
    Array<Color> result;
    int32 count = width * height;
    auto ptr = static_cast<uint8 *>(data);
    auto bytesPerPixel = Render::GetPixelSize(format);

    result.AppendUninitialized(count);
    for(int32 i = 0; i < count; ++i)
    {
        Render::UnpackColor(&result[i], format, ptr);
        ptr += bytesPerPixel;
    }
    return result;
}
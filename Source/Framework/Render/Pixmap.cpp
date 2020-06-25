#include "Render/Pixmap.h"

SPtr<Pixmap> Pixmap::Create(int32 width, int32 height, ResourceFormat format)
{
    return Memory::MakeShared<Pixmap>(width, height, format);
}

SPtr<Pixmap> Pixmap::Create(int32 width, int32 height, ResourceFormat format, const void *src)
{
    return Memory::MakeShared<Pixmap>(width, height, format, src);
}

Pixmap::Pixmap(int32 width, int32 height, ResourceFormat format)
    : width(width), height(height), format(format)
{
    CT_CHECK(width >= 0 && height >= 0);

    data = Memory::Alloc(GetSize());
    Fill(Color::WHITE);
}

Pixmap::Pixmap(int32 width, int32 height, ResourceFormat format, const void *src)
    : width(width), height(height), format(format)
{
    CT_CHECK(width >= 0 && height >= 0);

    auto size = GetSize();
    data = Memory::Alloc(size);
    std::memcpy(data, src, size);
}

Pixmap::~Pixmap()
{
    Memory::Free(data);
}

uint32 Pixmap::GetSize() const
{
    return width * height * GetResourceFormatBytes(format);
}

void Pixmap::Fill(const Color &color)
{
    //TODO
}

void Pixmap::SetPixel(int32 x, int32 y, const Color &color)
{
    //TODO
}

void Pixmap::SetPixels(const Color *src, int32 count)
{
    //TODO
}

void Pixmap::SetPixels(const Array<Color> &colors)
{
    //TODO
}

Color Pixmap::GetPixel(int32 x, int32 y)
{
    //TODO
    return Color();
}

Array<Color> Pixmap::GetPixels() const
{
    Array<Color> result;
    //TODO
    return result;
}

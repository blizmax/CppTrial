#pragma once

#include "Render/.Package.h"
#include "Math/Color.h"

class Pixmap
{
public:
    Pixmap(int32 width, int32 height, PixelFormat format);
    Pixmap(int32 width, int32 height, PixelFormat format, const void *src);
    ~Pixmap();

    void Fill(const Color &color);
    void SetColor(int32 x, int32 y, const Color &color);
    void SetColors(const Color *src, int32 count);
    void SetColors(const Array<Color> &colors);
    Color GetColor(int32 x, int32 y);
    Array<Color> GetColors() const;

    uint32 GetSize() const;

    const void *GetData() const
    {
        return data;
    }

    bool IsValid(int32 x, int32 y) const
    {
        return (x >= 0 && x < width) && (y >= 0 && y < height);
    }

    int32 GetWidth() const
    {
        return width;
    }

    int32 GetHeight() const
    {
        return height;
    }

    PixelFormat GetFormat() const
    {
        return format;
    }

    static SPtr<Pixmap> Create(int32 width, int32 height, PixelFormat format = PixelFormat::RGBA8888);
    static SPtr<Pixmap> Create(int32 width, int32 height, PixelFormat format, const void *src);

private:
    int32 width;
    int32 height;
    PixelFormat format;
    void *data = nullptr;
};
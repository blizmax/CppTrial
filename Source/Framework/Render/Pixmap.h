#pragma once

#include "Render/.Package.h"
#include "Math/Color.h"

class Pixmap
{
public:
    Pixmap(int32 width, int32 height, ResourceFormat format);
    Pixmap(int32 width, int32 height, ResourceFormat format, const void *src);
    ~Pixmap();

    void Fill(const Color &color);
    void SetPixel(int32 x, int32 y, const Color &color);
    void SetPixels(const Color *src, int32 count);
    void SetPixels(const Array<Color> &colors);
    Color GetPixel(int32 x, int32 y);
    Array<Color> GetPixels() const;

    uint32 GetSize() const;

    int32 GetWidth() const
    {
        return width;
    }

    int32 GetHeight() const
    {
        return height;
    }

    ResourceFormat GetResourceFormat() const
    {
        return format;
    }

    void *GetData() const
    {
        return data;
    }

    static SPtr<Pixmap> Create(int32 width, int32 height, ResourceFormat format = ResourceFormat::RGBA8Unorm);
    static SPtr<Pixmap> Create(int32 width, int32 height, ResourceFormat format, const void *src);

private:
    int32 width;
    int32 height;
    ResourceFormat format;
    void *data = nullptr;
};
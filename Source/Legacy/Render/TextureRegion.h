#pragma once

#include "Render/Texture.h"
#include "Math/Rect.h"

/** Contains a rectangular area of a texture, (x, y) is the left bottom point of the region.*/
class TextureRegion
{
public:
    TextureRegion() = delete;

    TextureRegion(const SPtr<Texture> &texture)
        : texture(texture), width(texture->GetWidth()), height(texture->GetHeight())
    {
    }

    TextureRegion(const SPtr<Texture> &texture, int32 x, int32 y, int32 w, int32 h)
        : texture(texture), x(x), y(y), width(w), height(h)
    {
    }

    TextureRegion(const SPtr<Texture> &texture, const Vector2 &uv0, const Vector2 &uv1)
        : texture(texture)
    {
        const int32 w = texture->GetWidth();
        const int32 h = texture->GetHeight();

        x = Math::RoundToInt(uv0.x * w);
        y = Math::RoundToInt(uv0.y * h);
        width = Math::RoundToInt(uv1.x * w) - x;
        height = Math::RoundToInt(uv1.y * h) - y;
    }

    const SPtr<Texture> &GetTexture() const
    {
        return texture;
    }

    int32 GetX() const
    {
        return x;
    }

    int32 GetY() const
    {
        return y;
    }

    int32 GetWidth() const
    {
        return width;
    }

    int32 GetHeight() const
    {
        return height;
    }

    Vector2 GetUV0() const
    {
        return Vector2(x / texture->GetWidth(), y / texture->GetHeight());
    }

    Vector2 GetUV1() const
    {
        return Vector2((x + width) / texture->GetWidth(), (y + height) / texture->GetHeight());
    }

    static Array<Array<TextureRegion>> Split(const TextureRegion &region, int32 tileWidth, int32 tileHeight)
    {
        const int32 cols = region.width / tileWidth;
        const int32 rows = region.height / tileHeight;
        Array<Array<TextureRegion>> result(rows);

        for (int32 r = 0; r < rows; ++r)
        {
            result.Add(Array<TextureRegion>(cols));

            int32 y = region.y + r * tileHeight;
            for (int32 c = 0; c < cols; ++c)
            {
                int32 x = region.x + c * tileWidth;
                result[r].Add(TextureRegion(region.texture, x, y, tileWidth, tileHeight));
            }
        }
        return result;
    }

    static Array<Array<TextureRegion>> Split(const SPtr<Texture> &texture, int32 tileWidth, int32 tileHeight)
    {
        return Split(TextureRegion(texture), tileWidth, tileHeight);
    }

private:
    SPtr<Texture> texture;
    int32 x = 0;
    int32 y = 0;
    int32 width, height;
};
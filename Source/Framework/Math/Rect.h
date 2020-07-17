#pragma once

#include "Math/.Package.h"
#include "Math/Vector2.h"

class Rect
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

public:
    Rect() = default;

    Rect(float x, float y, float w, float h)
        : x(x), y(y), width(w), height(h)
    {
    }

    Rect &SetPosition(float px, float py)
    {
        x = px;
        y = py;
        return *this;
    }

    Rect &SetPosition(const Vector2 &position)
    {
        x = position.x;
        y = position.y;
        return *this;
    }

    Vector2 GetPosition() const
    {
        return Vector2(x, y);
    }

    Rect &SetSize(float w, float h)
    {
        width = w;
        height = h;
        return *this;
    }

    Rect &SetSize(const Vector2 &size)
    {
        width = size.x;
        height = size.y;
        return *this;
    }

    Vector2 GetSize() const
    {
        return Vector2(width, height);
    }

    Rect &SetCenter(float px, float py)
    {
        x = px - width / 2;
        y = py - height / 2;
        return *this;
    }

    Rect &SetCenter(const Vector2 &center)
    {
        return SetCenter(center.x, center.y);
    }

    Vector2 GetCenter() const
    {
        return Vector2(x + width / 2, y + height / 2);
    }

    bool Contains(float px, float py) const
    {
        return x <= px && x + width >= px && y <= py && y + height >= py;
    }

    bool Contains(const Vector2 &point) const
    {
        return Contains(point.x, point.y);
    }

    bool Contains(const Rect &other) const
    {
        return Contains(other.x, other.y) && Contains(other.x + other.width, other.y + other.height);
    }

    bool Overlaps(const Rect &other) const
    {
        return x < other.x + other.width && x + width > other.x &&
               y < other.y + other.height && y + height > other.y;
    }

    Rect &Merge(float px, float py)
    {
        float minX = Math::Min(x, px);
        float maxX = Math::Max(x + width, px);
        x = minX;
        width = maxX - minX;

        float minY = Math::Min(y, py);
        float maxY = Math::Max(y + height, py);
        y = minY;
        height = maxY - minY;

        return *this;
    }

    Rect &Merge(const Vector2 &point)
    {
        return Merge(point.x, point.y);
    }

    Rect &Merge(const Rect &other)
    {
        float minX = Math::Min(x, other.x);
        float maxX = Math::Max(x + width, other.x + other.width);
        x = minX;
        width = maxX - minX;

        float minY = Math::Min(y, other.y);
        float maxY = Math::Max(y + height, other.y + other.height);
        y = minY;
        height = maxY - minY;

        return *this;
    }

    bool operator=(const Rect &other) const
    {
        return x == other.x && y == other.y && width == other.width && height == other.height;
    }

    bool operator!=(const Rect &other) const
    {
        return x != other.x || y != other.y || width != other.width || height != other.height;
    }

    String ToString() const
    {
        return String::Format(CT_TEXT("[x={0},y={1},width={2},height={3}]"), x, y, width, height);
    }

    uint32 HashCode() const
    {
        uint32 hash = Hash::HashValue(x);
        Hash::HashCombine(hash, Hash::HashValue(y));
        Hash::HashCombine(hash, Hash::HashValue(width));
        Hash::HashCombine(hash, Hash::HashValue(height));
        return hash;
    }
};
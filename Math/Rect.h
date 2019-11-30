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

    Rect(float x, float y, float w, float h) : x(x), y(y), width(w), height(h)
    {

    }

    Rect& SetPosition(float px, float py)
    {
        x = px;
        y = py;
        return *this;
    }

    Rect& SetPosition(const Vector2& position)
    {
        x = position.x;
        y = position.y;
        return *this;
    }

    Vector2 GetPosition() const
    {
        return Vector2(x, y);
    }

    Rect& SetSize(float w, float h)
    {
        width = w;
        height = h;
        return *this;
    }

    Rect& SetSize(const Vector2& size)
    {
        width = size.x;
        height = size.y;
        return *this;
    }

    Vector2 GetSize() const
    {
        return Vector2(width, height);
    }

    Rect& SetCenter(float px, float py)
    {
        //TODO
    }

    bool Contains(float px, float py) const
    {
        return x <= px && x + width >= px && y <= py && y + height >= py;
    }

    bool Contains(const Vector2& point) const
    {
        return Contains(point.x, point.y);
    }

    bool Overlaps(const Rect& other) const
    {
        return x < other.x + other.width && x + width > other.x &&
            y < other.y + other.height && y + height > other.y;
    }
};
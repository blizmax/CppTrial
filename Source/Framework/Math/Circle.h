#pragma once

#include "Math/.Package.h"
#include "Math/Vector2.h"

class Circle
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float radius = 0.0f;

public:
    Circle() = default;

    Circle(float x, float y, float radius) : x(x), y(y), radius(radius)
    {
    }

    Circle(const Vector2 &pos, float radius) : x(pos.x), y(pos.y), radius(radius)
    {
    }

    Circle &SetPosition(float px, float py)
    {
        x = px;
        y = py;
        return *this;
    }

    Circle &SetPosition(const Vector2 &position)
    {
        x = position.x;
        y = position.y;
        return *this;
    }

    Vector2 GetPosition() const
    {
        return Vector2(x, y);
    }

    bool Contains(float px, float py) const
    {
        float dx = x - px;
        float dy = y - py;
        return dx * dx + dy * dy <= radius * radius;
    }

    bool Contains(const Vector2 &point) const
    {
        return Contains(point.x, point.y);
    }

    bool Contains(const Circle &other) const
    {
        float radiusDiff = radius - other.radius;
        if (radiusDiff < 0.0f)
            return false;

        float dx = x - other.x;
        float dy = y - other.y;
        return dx * dx + dy * dy <= radiusDiff * radiusDiff;
    }

    bool Overlaps(const Circle &other) const
    {
        float dx = x - other.x;
        float dy = y - other.y;
        float radiusSum = radius + other.radius;
        return dx * dx + dy * dy < radiusSum * radiusSum;
    }

    bool operator=(const Circle &other) const
    {
        return x == other.x && y == other.y && radius == other.radius;
    }

    bool operator!=(const Circle &other) const
    {
        return x != other.x || y != other.y || radius != other.radius;
    }

    String ToString() const
    {
        return String::Format(CT_TEXT("[x={0},y={1},radius={2}]"), x, y, radius);
    }

    uint32 HashCode() const
    {
        uint32 hash = Hash::HashValue(x);
        Hash::HashCombine(hash, Hash::HashValue(y));
        Hash::HashCombine(hash, Hash::HashValue(radius));
        return hash;
    }
};
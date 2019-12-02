#pragma once

#include "Math/.Package.h"

class Vector2
{
public:
    float x = 0.0f;
    float y = 0.0f;

    static const Vector2 ZERO;
    static const Vector2 ONE;
    static const Vector2 X;
    static const Vector2 Y;

public:
    Vector2() = default;
    Vector2(const Vector2 &) = default;
    Vector2 &operator=(const Vector2 &) = default;

    Vector2(float x, float y) : x(x), y(y)
    {
    }

    float operator[](uint32 i) const
    {
        CT_ASSERT(i < 2);
        return *(&x + i);
    }

    float &operator[](uint32 i)
    {
        CT_ASSERT(i < 2);
        return *(&x + i);
    }

    Vector2 &operator+=(const Vector2 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2 &operator+=(float rhs)
    {
        x += rhs;
        y += rhs;
        return *this;
    }

    Vector2 &operator-=(const Vector2 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2 &operator-=(float rhs)
    {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    Vector2 &operator*=(const Vector2 &rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    Vector2 &operator*=(float rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    Vector2 &operator/=(const Vector2 &rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    Vector2 &operator/=(float rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    bool operator==(const Vector2 &other) const
    {
        return (x == other.x && y == other.y);
    }

    bool operator!=(const Vector2 &other) const
    {
        return (x != other.y || y != other.y);
    }

    Vector2 operator+() const
    {
        return *this;
    }

    Vector2 operator-() const
    {
        return Vector2(-x, -y);
    }

    friend Vector2 operator+(const Vector2 &lhs, const Vector2 &rhs)
    {
        return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    friend Vector2 operator-(const Vector2 &lhs, const Vector2 &rhs)
    {
        return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    friend Vector2 operator*(const Vector2 &lhs, const Vector2 &rhs)
    {
        return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
    }

    friend Vector2 operator*(float lhs, const Vector2 &rhs)
    {
        return Vector2(lhs * rhs.x, lhs * rhs.y);
    }

    friend Vector2 operator*(const Vector2 &lhs, float rhs)
    {
        return Vector2(lhs.x * rhs, lhs.y * rhs);
    }

    friend Vector2 operator/(const Vector2 &lhs, const Vector2 &rhs)
    {
        return Vector2(lhs.x / rhs.x, lhs.y / rhs.y);
    }

    friend Vector2 operator/(float lhs, const Vector2 &rhs)
    {
        return Vector2(lhs / rhs.x, lhs / rhs.y);
    }

    friend Vector2 operator/(const Vector2 &lhs, float rhs)
    {
        return Vector2(lhs.x / rhs, lhs.y / rhs);
    }

    float Length2() const
    {
        return (x * x + y * y);
    }

    float Length() const
    {
        return Math::Sqrt(Length2());
    }

    float Distance2(const Vector2 &other) const
    {
        return (*this - other).Length2();
    }

    float Distance(const Vector2 &other) const
    {
        return (*this - other).Length();
    }

    float Dot(const Vector2 &other) const
    {
        return x * other.x + y * other.y;
    }

    float Cross(const Vector2 &other) const
    {
        return x * other.y - y * other.x;
    }

    Vector2 &Normalize()
    {
        float len = Length();
        if (len < Math::EPSILON)
        {
            x = 0.0f;
            y = 0.0f;
        }
        else
        {
            x /= len;
            y /= len;
        }
        return *this;
    }

    bool IsZero() const
    {
        float len = Length();
        return len < Math::EPSILON;
    }

    bool IsPerpendicular(const Vector2 &other) const
    {
        return Dot(other) < Math::EPSILON;
    }

    String ToString() const
    {
        return String::Format(CT_TEXT("[x={0},y={1}]"), x, y);
    }

    uint32 HashCode() const
    {
        uint32 hash = Hash::HashValue(x);
        Hash::HashCombine(hash, Hash::HashValue(y));
        return hash;
    }
};

#pragma once

#include "Math/.Package.h"

class Vector4
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

public:
    Vector4() = default;

    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
    {
    }

    float operator[](uint32 i) const
    {
        CT_ASSERT(i < 4);
        return *(&x + i);
    }

    float &operator[](uint32 i)
    {
        CT_ASSERT(i < 4);
        return *(&x + i);
    }

    Vector4 &operator+=(const Vector4 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vector4 &operator+=(float rhs)
    {
        x += rhs;
        y += rhs;
        z += rhs;
        w += rhs;
        return *this;
    }

    Vector4 &operator-=(const Vector4 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Vector4 &operator-=(float rhs)
    {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        w -= rhs;
        return *this;
    }

    Vector4 &operator*=(const Vector4 &rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }

    Vector4 &operator*=(float rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    Vector4 &operator/=(const Vector4 &rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        w /= rhs.w;
        return *this;
    }

    Vector4 &operator/=(float rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    bool operator==(const Vector4 &other) const
    {
        return (x == other.x && y == other.y && z == other.z && w == other.w);
    }

    bool operator!=(const Vector4 &other) const
    {
        return (x != other.y || y != other.y || z != other.z || w != other.w);
    }

    Vector4 operator+() const
    {
        return *this;
    }

    Vector4 operator-() const
    {
        return Vector4(-x, -y, -z, -w);
    }

    friend Vector4 operator+(const Vector4 &lhs, const Vector4 &rhs)
    {
        return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    friend Vector4 operator-(const Vector4 &lhs, const Vector4 &rhs)
    {
        return Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    }

    friend Vector4 operator*(const Vector4 &lhs, const Vector4 &rhs)
    {
        return Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
    }

    friend Vector4 operator*(float lhs, const Vector4 &rhs)
    {
        return Vector4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
    }

    friend Vector4 operator*(const Vector4 &lhs, float rhs)
    {
        return Vector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
    }

    friend Vector4 operator/(const Vector4 &lhs, const Vector4 &rhs)
    {
        return Vector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
    }

    friend Vector4 operator/(float lhs, const Vector4 &rhs)
    {
        return Vector4(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w);
    }

    friend Vector4 operator/(const Vector4 &lhs, float rhs)
    {
        return Vector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
    }

    float Length2() const
    {
        return (x * x + y * y + z * z + w * w);
    }

    float Length() const
    {
        return Math::Sqrt(Length2());
    }

    float Distance2(const Vector4 &other) const
    {
        return (*this - other).Length2();
    }

    float Distance(const Vector4 &other) const
    {
        return (*this - other).Length();
    }

    float Dot(const Vector4 &other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector4 &Normalize()
    {
        float len = Length();
        if (len < Math::EPSILON)
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
            w = 0.0f;
        }
        else
        {
            x /= len;
            y /= len;
            z /= len;
            w /= len;
        }
        return *this;
    }

    bool IsZero() const
    {
        float len = Length();
        return len < Math::EPSILON;
    }

    bool IsPerpendicular(const Vector4 &other) const
    {
        return Dot(other) < Math::EPSILON;
    }

    String ToString() const
    {
        return String::Format(CT_TEXT("x={0},y={1},z={2},w={3}"), x, y, z, w);
    }

    uint32 HashCode() const
    {
        uint32 hash = Hash::HashValue(x);
        Hash::HashCombine(hash, Hash::HashValue(y));
        Hash::HashCombine(hash, Hash::HashValue(z));
        Hash::HashCombine(hash, Hash::HashValue(w));
        return hash;
    }
};
#pragma once

#include "Math/.Package.h"
#include "Math/Vector2.h"

class Vector3
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    static const Vector3 ZERO;
    static const Vector3 ONE;
    static const Vector3 X;
    static const Vector3 Y;
    static const Vector3 Z;

public:
    Vector3() = default;
    Vector3(const Vector3 &) = default;
    Vector3 &operator=(const Vector3 &) = default;

    Vector3(float x, float y, float z)
        : x(x), y(y), z(z)
    {
    }

    explicit operator Vector2() const
    {
        return Vector2(x, y);
    }

    float operator[](int32 i) const
    {
        CT_CHECK(i >= 0 && i < 3);
        return *(&x + i);
    }

    float &operator[](int32 i)
    {
        CT_CHECK(i >= 0 && i < 3);
        return *(&x + i);
    }

    Vector3 &operator+=(const Vector3 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vector3 &operator+=(float rhs)
    {
        x += rhs;
        y += rhs;
        z += rhs;
        return *this;
    }

    Vector3 &operator-=(const Vector3 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vector3 &operator-=(float rhs)
    {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        return *this;
    }

    Vector3 &operator*=(const Vector3 &rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    Vector3 &operator*=(float rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    Vector3 &operator/=(const Vector3 &rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    Vector3 &operator/=(float rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    bool operator==(const Vector3 &other) const
    {
        return (x == other.x && y == other.y && z == other.z);
    }

    bool operator!=(const Vector3 &other) const
    {
        return (x != other.y || y != other.y || z != other.z);
    }

    Vector3 operator+() const
    {
        return *this;
    }

    Vector3 operator-() const
    {
        return Vector3(-x, -y, -z);
    }

    friend Vector3 operator+(const Vector3 &lhs, const Vector3 &rhs)
    {
        return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }

    friend Vector3 operator-(const Vector3 &lhs, const Vector3 &rhs)
    {
        return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }

    friend Vector3 operator*(const Vector3 &lhs, const Vector3 &rhs)
    {
        return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
    }

    friend Vector3 operator*(float lhs, const Vector3 &rhs)
    {
        return Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
    }

    friend Vector3 operator*(const Vector3 &lhs, float rhs)
    {
        return Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
    }

    friend Vector3 operator/(const Vector3 &lhs, const Vector3 &rhs)
    {
        return Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
    }

    friend Vector3 operator/(float lhs, const Vector3 &rhs)
    {
        return Vector3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
    }

    friend Vector3 operator/(const Vector3 &lhs, float rhs)
    {
        return Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
    }

    float Length2() const
    {
        return (x * x + y * y + z * z);
    }

    float Length() const
    {
        return Math::Sqrt(Length2());
    }

    float Distance2(const Vector3 &other) const
    {
        return (*this - other).Length2();
    }

    float Distance(const Vector3 &other) const
    {
        return (*this - other).Length();
    }

    float Dot(const Vector3 &other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 Cross(const Vector3 &other) const
    {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x);
    }

    Vector3 &Normalize()
    {
        float len = Length();
        if (len < Math::EPSILON)
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }
        else
        {
            x /= len;
            y /= len;
            z /= len;
        }
        return *this;
    }

    bool IsZero() const
    {
        float len = Length();
        return len < Math::EPSILON;
    }

    bool IsPerpendicular(const Vector3 &other) const
    {
        return Dot(other) < Math::EPSILON;
    }

    static Vector3 Lerp(const Vector3 &a, const Vector3 &b, float t)
    {
        return Vector3(Math::Lerp(a.x, b.x, t), Math::Lerp(a.y, b.y, t), Math::Lerp(a.z, b.z, t));
    }

    static Vector3 Max(const Vector3 &a, const Vector3 &b)
    {
        return Vector3(Math::Max(a.x, b.x), Math::Max(a.y, b.y), Math::Max(a.z, b.z));
    }

    static Vector3 Min(const Vector3 &a, const Vector3 &b)
    {
        return Vector3(Math::Min(a.x, b.x), Math::Min(a.y, b.y), Math::Min(a.z, b.z));
    }

    String ToString() const
    {
        return String::Format(CT_TEXT("[x={0},y={1},z={2}]"), x, y, z);
    }

    uint32 HashCode() const
    {
        uint32 hash = Hash::HashValue(x);
        Hash::HashCombine(hash, Hash::HashValue(y));
        Hash::HashCombine(hash, Hash::HashValue(z));
        return hash;
    }
};

inline const Vector3 Vector3::ZERO(0.0f, 0.0f, 0.0f);
inline const Vector3 Vector3::ONE(1.0f, 1.0f, 1.0f);
inline const Vector3 Vector3::X(1.0f, 0.0f, 0.0f);
inline const Vector3 Vector3::Y(0.0f, 1.0f, 0.0f);
inline const Vector3 Vector3::Z(0.0f, 0.0f, 1.0f);
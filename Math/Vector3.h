#pragma once

#include "Math/.Package.h"

class Vector3
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

public:
    Vector3() = default;

    Vector3(float x, float y, float z) : x(x), y(y), z(z)
    {
    }

    float operator[](uint32 i) const
    {
        CT_ASSERT(i < 3);
        return *(&x + i);
    }

    float &operator[](uint32 i)
    {
        CT_ASSERT(i < 3);
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

    Vector3 Cross(const Vector3& other) const
    {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
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

    String ToString() const
    {
        return String::Format(CT_TEXT("x={0},y={1},z={2}"), x, y, z);
    }

    uint32 HashCode() const
    {
        uint32 hash = Hash::HashValue(x);
        Hash::HashCombine(hash, Hash::HashValue(y));
        Hash::HashCombine(hash, Hash::HashValue(z));
        return hash;
    }
};
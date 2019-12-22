#pragma once

#include "Math/.Package.h"

class Quat
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;

    Quat() = default;

    Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
    {
    }

    Quat(const Matrix3 &mat)
    {
        FromMatrix(mat);
    }

    Quat(const Vector3 &axis, float angle)
    {
        FromAxisAngle(axis, angle);
    }

    Quat(float yaw, float pitch, float roll)
    {
        FromEulerAngles(yaw, pitch, roll);
    }

    //TODO
    Quat &FromMatrix(const Matrix3 &mat);
    Quat &FromAxisAngle(const Vector3 &axis, float angle);
    Quat &FromEulerAngles(float yaw, float pitch, float roll);

    Quat &Normalize()
    {
        float len = Length();
        if (len < Math::EPSILON)
        {
            x = y = z = w = 0.0f;
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

    Quat &Indentity()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 1.0f;
        return *this;
    }

    Quat &Conjugate()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    float Length2() const
    {
        return x * x + y * y + z * z + w * w;
    }

    float Length() const
    {
        return Math::Sqrt(Length2());
    }

    float Dot(const Quat &other) const
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
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

    Quat &operator+=(const Quat &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Quat &operator-=(const Quat &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Quat &operator*=(const Quat &rhs)
    {
        float x0 = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
        float y0 = w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z;
        float z0 = w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x;
        float w0 = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;

        x = x0;
        y = y0;
        z = z0;
        w = w0;

        return *this;
    }

    Quat &operator*=(float rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    Quat &operator/=(float rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    bool operator==(const Quat &other) const
    {
        return (x == other.x && y == other.y && z == other.z && w == other.w);
    }

    bool operator!=(const Quat &other) const
    {
        return (x != other.x || y != other.y || z != other.z || w != other.w);
    }

    Quat operator+()
    {
        return *this;
    }

    Quat operator-()
    {
        return Quat(-x, -y, -z, -w);
    }

    friend Quat operator+(const Quat &lhs, const Quat &rhs)
    {
        return Quat(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    friend Quat operator-(const Quat &lhs, const Quat &rhs)
    {
        return Quat(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    }

    friend Quat operator*(const Quat &lhs, const Quat &rhs)
    {
        return Quat(
            lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
            lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
    }

    friend Quat operator*(float lhs, const Quat &rhs)
    {
        return Quat(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
    }

    friend Quat operator*(const Quat &lhs, float rhs)
    {
        return Quat(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
    }

    friend Quat operator/(float lhs, const Quat &rhs)
    {
        return Quat(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w);
    }

    friend Quat operator/(const Quat &lhs, float rhs)
    {
        return Quat(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
    }

    String ToString() const
    {
        return String::Format(CT_TEXT("[x={0},y={1},z={2},w={3}]"), x, y, z, w);
    }
};
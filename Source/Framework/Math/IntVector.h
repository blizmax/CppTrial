#pragma once

#include "Math/.Package.h"

template <typename T>
class IntVector2
{
public:
    T x = 0;
    T y = 0;

    IntVector2() = default;
    IntVector2(const IntVector2 &) = default;
    IntVector2 &operator=(const IntVector2 &) = default;

    IntVector2(T x, T y) : x(x), y(y)
    {
    }

    IntVector2(T v) : x(v), y(v)
    {
    }

    T operator[](int32 i) const
    {
        CT_CHECK(i >= 0 && i < 2);
        return *(&x + i);
    }

    T &operator[](int32 i)
    {
        CT_CHECK(i >= 0 && i < 2);
        return *(&x + i);
    }
};

template <typename T>
class IntVector3
{
public:
    T x = 0;
    T y = 0;
    T z = 0;

    IntVector3() = default;
    IntVector3(const IntVector3 &) = default;
    IntVector3 &operator=(const IntVector3 &) = default;

    IntVector3(T x, T y, T z) : x(x), y(y), z(z)
    {
    }

    IntVector3(T v) : x(v), y(v), z(v)
    {
    }

    T operator[](int32 i) const
    {
        CT_CHECK(i >= 0 && i < 3);
        return *(&x + i);
    }

    T &operator[](int32 i)
    {
        CT_CHECK(i >= 0 && i < 3);
        return *(&x + i);
    }
};

template <typename T>
class IntVector4
{
public:
    T x = 0;
    T y = 0;
    T z = 0;
    T w = 0;

    IntVector4() = default;
    IntVector4(const IntVector4 &) = default;
    IntVector4 &operator=(const IntVector4 &) = default;

    IntVector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
    {
    }

    IntVector4(T v) : x(v), y(v), z(v), w(v)
    {
    }

    T operator[](int32 i) const
    {
        CT_CHECK(i >= 0 && i < 4);
        return *(&x + i);
    }

    T &operator[](int32 i)
    {
        CT_CHECK(i >= 0 && i < 4);
        return *(&x + i);
    }
};

using Vector2I = IntVector2<int32>;
using Vector2U = IntVector2<uint32>;
using Vector3I = IntVector3<int32>;
using Vector3U = IntVector3<uint32>;
using Vector4I = IntVector4<int32>;
using Vector4U = IntVector4<uint32>;

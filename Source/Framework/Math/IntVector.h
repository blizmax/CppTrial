#pragma once

#include "Math/.Package.h"

template <typename T>
class TIVector2
{
public:
    T x = 0;
    T y = 0;

    TIVector2() = default;
    TIVector2(const TIVector2 &) = default;
    TIVector2 &operator=(const TIVector2 &) = default;

    TIVector2(T x, T y) : x(x), y(y)
    {
    }
};

template <typename T>
class TIVector3
{
public:
    T x = 0;
    T y = 0;
    T z = 0;

    TIVector3() = default;
    TIVector3(const TIVector3 &) = default;
    TIVector3 &operator=(const TIVector3 &) = default;

    TIVector3(T x, T y, T z) : x(x), y(y), z(z)
    {
    }
};

template <typename T>
class TIVector4
{
public:
    T x = 0;
    T y = 0;
    T z = 0;
    T w = 0;

    TIVector4() = default;
    TIVector4(const TIVector4 &) = default;
    TIVector4 &operator=(const TIVector4 &) = default;

    TIVector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
    {
    }
};

using IVector2 = TIVector2<int32>;
using UVector2 = TIVector2<uint32>;
using IVector3 = TIVector3<int32>;
using UVector3 = TIVector3<uint32>;
using IVector4 = TIVector4<int32>;
using UVector4 = TIVector4<uint32>;


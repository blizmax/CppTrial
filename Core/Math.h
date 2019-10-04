#pragma once

#include "Core/General.h"
#include <cmath>

CT_SCOPE_BEGIN

namespace Math
{

CT_INLINE float Sin(float value)
{
    return std::sin(value);
}

CT_INLINE double Sin(double value)
{
    return std::sin(value);
}

CT_INLINE float Cos(float value)
{
    return std::cos(value);
}

CT_INLINE double Cos(double value)
{
    return std::cos(value);
}

CT_INLINE float Tan(float value)
{
    return std::tan(value);
}

CT_INLINE double Tan(double value)
{
    return std::tan(value);
}

CT_INLINE float Asin(float value)
{
    return std::asin(value);
}

CT_INLINE double Asin(double value)
{
    return std::asin(value);
}

CT_INLINE float Acos(float value)
{
    return std::acos(value);
}

CT_INLINE double Acos(double value)
{
    return std::acos(value);
}

CT_INLINE float Atan(float value)
{
    return std::atan(value);
}

CT_INLINE double Atan(double value)
{
    return std::atan(value);
}

CT_INLINE float Atan2(float y, float x)
{
    return std::atan2(y, x);
}

CT_INLINE double Atan2(double y, double x)
{
    return std::atan2(y, x);
}

CT_INLINE float Abs(float value)
{
    return std::fabs(value);
}

CT_INLINE double Abs(double value)
{
    return std::fabs(value);
}

CT_INLINE int32 Abs(int32 value)
{
    return std::abs(value);
}

CT_INLINE int64 Abs(int64 value)
{
    return std::abs(value);
}

CT_INLINE float Ceil(float value)
{
    return std::ceil(value);
}

CT_INLINE double Ceil(double value)
{
    return std::ceil(value);
}

CT_INLINE float Floor(float value)
{
    return std::floor(value);
}

CT_INLINE double Floor(double value)
{
    return std::floor(value);
}

CT_INLINE float Exp(float value)
{
    return std::exp(value);
}

CT_INLINE double Exp(double value)
{
    return std::exp(value);
}

} // namespace Math

CT_SCOPE_END
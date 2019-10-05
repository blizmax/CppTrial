#pragma once

#include "Core/General.h"
#include <cmath>

CT_SCOPE_BEGIN

namespace Math
{

constexpr float PI = 3.14159265359f;
constexpr float TWO_PI = PI * 2.0f;
constexpr float HALF_PI = PI * 0.5f;
constexpr float RadToDeg = 180.0f / PI;
constexpr float DegToRad = PI / 180.0f;

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

CT_INLINE int32 TruncToInt(float value)
{
    return (int32)value;
}

CT_INLINE int64 TruncToInt(double value)
{
    return (int64)value;
}

CT_INLINE float Trunc(float value)
{
    return (float)TruncToInt(value);
}

CT_INLINE double Trunc(double value)
{
    return (double)TruncToInt(value);
}

CT_INLINE float Ceil(float value)
{
    return std::ceil(value);
}

CT_INLINE double Ceil(double value)
{
    return std::ceil(value);
}

CT_INLINE int32 CeilToInt(float value)
{
    return TruncToInt(Ceil(value));
}

CT_INLINE int64 CeilToInt(double value)
{
    return TruncToInt(Ceil(value));
}

CT_INLINE float Floor(float value)
{
    return std::floor(value);
}

CT_INLINE double Floor(double value)
{
    return std::floor(value);
}

CT_INLINE int32 FloorToInt(float value)
{
    return TruncToInt(Floor(value));
}

CT_INLINE int64 FloorToInt(double value)
{
    return TruncToInt(Floor(value));
}

CT_INLINE float Round(float value)
{
    return std::round(value);
}

CT_INLINE double Round(double value)
{
    return std::round(value);
}

CT_INLINE int32 RoundToInt(float value)
{
    return FloorToInt(value + 0.5f);
}

CT_INLINE int64 RoundToInt(double value)
{
    return FloorToInt(value + 0.5);
}

CT_INLINE float Sqrt(float value)
{
    return std::sqrt(value);
}

CT_INLINE double Sqrt(double value)
{
    return std::sqrt(value);
}

CT_INLINE float Exp(float value)
{
    return std::exp(value);
}

CT_INLINE double Exp(double value)
{
    return std::exp(value);
}

CT_INLINE float Exp2(float value)
{
    return std::exp2(value);
}

CT_INLINE double Exp2(double value)
{
    return std::exp2(value);
}

CT_INLINE float Pow(float a, float b)
{
    return std::pow(a, b);
}

CT_INLINE double Pow(double a, double b)
{
    return std::pow(a, b);
}

CT_INLINE float Loge(float value)
{
    return std::log(value);
}

CT_INLINE double Loge(double value)
{
    return std::log(value);
}

CT_INLINE float Log2(float value)
{
    return std::log2(value);
}

CT_INLINE double Log2(double value)
{
    return std::log2(value);
}

CT_INLINE float Log10(float value)
{
    return std::log10(value);
}

CT_INLINE double Log10(double value)
{
    return std::log10(value);
}

// Return the remainder of a/b
CT_INLINE float Fmod(float a, float b)
{
    return std::fmod(a, b);
}

// Return the remainder of x/y
CT_INLINE double Fmod(double a, double b)
{
    return std::fmod(a, b);
}

// Return the frac part of the number, and use a ptr to receive the int part
CT_INLINE float Modf(float value, float *intPart)
{
    return std::modf(value, intPart);
}

// Return the frac part of the number, and use a ptr to receive the int part
CT_INLINE double Modf(double value, double *intPart)
{
    return std::modf(value, intPart);
}

template <typename T>
CT_INLINE T Abs(T value)
{
    return (value < 0) ? -value : value;
}

template <typename T>
CT_INLINE auto Max(T value)
{
    return value;
}

template <typename T1, typename... T2>
CT_INLINE auto Max(T1 a, T2... b)
{
    auto max = Max(b...);
    return (max < a) ? a : max;
}

template <typename T>
CT_INLINE auto Min(T value)
{
    return value;
}

template <typename T1, typename... T2>
CT_INLINE auto Min(T1 a, T2... b)
{
    auto min = Min(b...);
    return (a < min) ? a : min;
}

template <typename T>
CT_INLINE T Clamp(T value, T min, T max)
{
    CT_ASSERT(!(max < min));
    return Max(Min(value, max), min);
}

CT_INLINE float Clamp01(float value)
{
    return Clamp(value, 0.0f, 1.0f);
}

CT_INLINE double Clamp01(double value)
{
    return Clamp(value, 0.0, 1.0);
}

} // namespace Math

CT_SCOPE_END
#pragma once

#include "Core/.Package.h"
#include <cmath>
#include <random>

class Math
{
public:
    static constexpr float EPSILON = 0.00001f;
    static constexpr float PI = 3.14159265359f;
    static constexpr float TWO_PI = PI * 2.0f;
    static constexpr float HALF_PI = PI * 0.5f;
    static constexpr float RAD_TO_DEG = 180.0f / PI;
    static constexpr float DEG_TO_RAD = PI / 180.0f;

private:
    static std::mt19937 &RandomGenerator()
    {
        static std::random_device seed;
        static std::mt19937 generator(seed());
        return generator;
    }

public:
    static float Sin(float value)
    {
        return std::sin(value);
    }

    static double Sin(double value)
    {
        return std::sin(value);
    }

    static float Cos(float value)
    {
        return std::cos(value);
    }

    static double Cos(double value)
    {
        return std::cos(value);
    }

    static float Tan(float value)
    {
        return std::tan(value);
    }

    static double Tan(double value)
    {
        return std::tan(value);
    }

    static float Asin(float value)
    {
        return std::asin(value);
    }

    static double Asin(double value)
    {
        return std::asin(value);
    }

    static float Acos(float value)
    {
        return std::acos(value);
    }

    static double Acos(double value)
    {
        return std::acos(value);
    }

    static float Atan(float value)
    {
        return std::atan(value);
    }

    static double Atan(double value)
    {
        return std::atan(value);
    }

    static float Atan2(float y, float x)
    {
        return std::atan2(y, x);
    }

    static double Atan2(double y, double x)
    {
        return std::atan2(y, x);
    }

    static int32 TruncToInt(float value)
    {
        return (int32)value;
    }

    static int64 TruncToInt(double value)
    {
        return (int64)value;
    }

    static float Trunc(float value)
    {
        return (float)TruncToInt(value);
    }

    static double Trunc(double value)
    {
        return (double)TruncToInt(value);
    }

    static float Ceil(float value)
    {
        return std::ceil(value);
    }

    static double Ceil(double value)
    {
        return std::ceil(value);
    }

    static int32 CeilToInt(float value)
    {
        return TruncToInt(Ceil(value));
    }

    static int64 CeilToInt(double value)
    {
        return TruncToInt(Ceil(value));
    }

    static float Floor(float value)
    {
        return std::floor(value);
    }

    static double Floor(double value)
    {
        return std::floor(value);
    }

    static int32 FloorToInt(float value)
    {
        return TruncToInt(Floor(value));
    }

    static int64 FloorToInt(double value)
    {
        return TruncToInt(Floor(value));
    }

    static float Round(float value)
    {
        return std::round(value);
    }

    static double Round(double value)
    {
        return std::round(value);
    }

    static int32 RoundToInt(float value)
    {
        return FloorToInt(value + 0.5f);
    }

    static int64 RoundToInt(double value)
    {
        return FloorToInt(value + 0.5);
    }

    static float Sqrt(float value)
    {
        return std::sqrt(value);
    }

    static double Sqrt(double value)
    {
        return std::sqrt(value);
    }

    static float Exp(float value)
    {
        return std::exp(value);
    }

    static double Exp(double value)
    {
        return std::exp(value);
    }

    static float Exp2(float value)
    {
        return std::exp2(value);
    }

    static double Exp2(double value)
    {
        return std::exp2(value);
    }

    static float Pow(float a, float b)
    {
        return std::pow(a, b);
    }

    static double Pow(double a, double b)
    {
        return std::pow(a, b);
    }

    static float Loge(float value)
    {
        return std::log(value);
    }

    static double Loge(double value)
    {
        return std::log(value);
    }

    static float Log2(float value)
    {
        return std::log2(value);
    }

    static double Log2(double value)
    {
        return std::log2(value);
    }

    static float Log10(float value)
    {
        return std::log10(value);
    }

    static double Log10(double value)
    {
        return std::log10(value);
    }

    // Return the remainder of a/b
    static float Fmod(float a, float b)
    {
        return std::fmod(a, b);
    }

    // Return the remainder of x/y
    static double Fmod(double a, double b)
    {
        return std::fmod(a, b);
    }

    // Return the frac part of the number, and use a ptr to receive the int part
    static float Modf(float value, float *intPart)
    {
        return std::modf(value, intPart);
    }

    // Return the frac part of the number, and use a ptr to receive the int part
    static double Modf(double value, double *intPart)
    {
        return std::modf(value, intPart);
    }

    template <typename T>
    static T Abs(T value)
    {
        return (value < 0) ? -value : value;
    }

    template <typename T>
    static auto Max(T value)
    {
        return value;
    }

    template <typename T1, typename... T2>
    static auto Max(T1 a, T2... b)
    {
        auto max = Max(b...);
        return (max < a) ? a : max;
    }

    template <typename T>
    static auto Min(T value)
    {
        return value;
    }

    template <typename T1, typename... T2>
    static auto Min(T1 a, T2... b)
    {
        auto min = Min(b...);
        return (a < min) ? a : min;
    }

    template <typename T>
    static T Clamp(T value, T min, T max)
    {
        CT_ASSERT(!(max < min));
        return Max(Min(value, max), min);
    }

    static float Clamp01(float value)
    {
        return Clamp(value, 0.0f, 1.0f);
    }

    static double Clamp01(double value)
    {
        return Clamp(value, 0.0, 1.0);
    }

    template <typename T>
    static bool IsPowerOfTwo(T value)
    {
        return (value & (value - 1)) == (T)0;
    }

    static uint64 NextPowerOfTwo(uint64 value)
    {
        uint64 pos = 0;
        uint64 a = 1;
        if (value >= a << 32)
        {
            value = value >> 32;
            pos += 32;
        }
        if (value >= a << 16)
        {
            value = value >> 16;
            pos += 16;
        }
        if (value >= a << 8)
        {
            value = value >> 8;
            pos += 8;
        }
        if (value >= a << 4)
        {
            value = value >> 4;
            pos += 4;
        }
        if (value >= a << 2)
        {
            value = value >> 2;
            pos += 2;
        }
        if (value >= a << 1)
        {
            pos += 1;
        }

        ++pos;
        return pos == 1 ? 2 : a << pos;
    }

    // Range[0, bound], a neg bound will be set to 0
    static int32 RandInt(int32 bound)
    {
        bound = bound < 0 ? 0 : bound;
        std::uniform_int_distribution<int32> dist(0, bound);
        return dist(RandomGenerator());
    }

    // Range[min(a, b), max(a, b)]
    static int32 RandInt(int32 a, int32 b)
    {
        if (a < b)
        {
            std::uniform_int_distribution<int32> dist(a, b);
            return dist(RandomGenerator());
        }
        else
        {
            std::uniform_int_distribution<int32> dist(a, b);
            return dist(RandomGenerator());
        }
    }

    // Range[0.0, 1.0)
    static float Rand01()
    {
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(RandomGenerator());
    }

    // Range[0, max(uint32)]
    static uint32 Rand()
    {
        return RandomGenerator()();
    }

    static void RandSeed(uint32 seed)
    {
        RandomGenerator().seed(seed);
    }
};
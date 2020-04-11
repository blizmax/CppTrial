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
    static constexpr float Sin(float value)
    {
        return std::sin(value);
    }

    static constexpr float Cos(float value)
    {
        return std::cos(value);
    }

    static constexpr float Tan(float value)
    {
        return std::tan(value);
    }

    static constexpr float Asin(float value)
    {
        return std::asin(value);
    }

    static constexpr float Acos(float value)
    {
        return std::acos(value);
    }

    static constexpr float Atan(float value)
    {
        return std::atan(value);
    }

    static constexpr float Atan2(float y, float x)
    {
        return std::atan2(y, x);
    }

    static constexpr int32 TruncToInt(float value)
    {
        return (int32)value;
    }

    static constexpr float Trunc(float value)
    {
        return (float)TruncToInt(value);
    }

    static constexpr float Ceil(float value)
    {
        return std::ceil(value);
    }

    static constexpr int32 CeilToInt(float value)
    {
        return TruncToInt(Ceil(value));
    }

    static constexpr float Floor(float value)
    {
        return std::floor(value);
    }

    static constexpr int32 FloorToInt(float value)
    {
        return TruncToInt(Floor(value));
    }

    static constexpr float Round(float value)
    {
        return std::round(value);
    }

    static constexpr int32 RoundToInt(float value)
    {
        return FloorToInt(value + 0.5f);
    }

    static constexpr float Sqrt(float value)
    {
        return std::sqrt(value);
    }

    static constexpr float Exp(float value)
    {
        return std::exp(value);
    }

    static constexpr float Exp2(float value)
    {
        return std::exp2(value);
    }

    static constexpr float Pow(float a, float b)
    {
        return std::pow(a, b);
    }

    static constexpr float Loge(float value)
    {
        return std::log(value);
    }

    static constexpr float Log2(float value)
    {
        return std::log2(value);
    }

    static constexpr float Log10(float value)
    {
        return std::log10(value);
    }

    // Return the remainder of a/b
    static constexpr float Fmod(float a, float b)
    {
        return std::fmod(a, b);
    }

    // Return the frac part of the number, and use a ptr to receive the int part
    static CT_INLINE float Modf(float value, float *intPart)
    {
        return std::modf(value, intPart);
    }

    template <typename T>
    static constexpr T Abs(T value)
    {
        return (value < 0) ? -value : value;
    }

    template <typename T>
    static constexpr auto Max(T value)
    {
        return value;
    }

    template <typename T1, typename... T2>
    static constexpr auto Max(T1 a, T2... b)
    {
        auto max = Max(b...);
        return (max < a) ? a : max;
    }

    template <typename T>
    static constexpr auto Min(T value)
    {
        return value;
    }

    template <typename T1, typename... T2>
    static constexpr auto Min(T1 a, T2... b)
    {
        auto min = Min(b...);
        return (a < min) ? a : min;
    }

    template <typename T>
    static CT_INLINE T Clamp(T value, T min, T max)
    {
        CT_CHECK(!(max < min));
        return Max(Min(value, max), min);
    }

    static CT_INLINE float Clamp01(float value)
    {
        return Clamp(value, 0.0f, 1.0f);
    }

    template <typename T>
    static constexpr bool IsPowerOfTwo(T value)
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
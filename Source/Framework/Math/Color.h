#pragma once

#include "Core/Math.h"
#include "Math/Vector3.h"

class Color
{
public:
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;

    static const Color WHITE;
    static const Color BLACK;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;

    Color() = default;
    Color(const Color &) = default;
    Color &operator=(const Color &) = default;

    Color(float r, float g, float b, float a = 1.0f)
        : r(r), g(g), b(b), a(a)
    {
    }

    float operator[](int32 i) const
    {
        CT_CHECK(i >= 0 && i < 4);
        return *(&r + i);
    }

    float &operator[](int32 i)
    {
        CT_CHECK(i >= 0 && i < 4);
        return *(&r + i);
    }

    Color &operator+=(const Color &rhs)
    {
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        a += rhs.a;
        return *this;
    }

    Color &operator-=(const Color &rhs)
    {
        r -= rhs.r;
        g -= rhs.g;
        b -= rhs.b;
        a -= rhs.a;
        return *this;
    }

    Color &operator*=(const Color &rhs)
    {
        r *= rhs.r;
        g *= rhs.g;
        b *= rhs.b;
        a *= rhs.a;
        return *this;
    }

    Color &operator*=(float rhs)
    {
        r *= rhs;
        g *= rhs;
        b *= rhs;
        a *= rhs;
        return *this;
    }

    Color &operator/=(const Color &rhs)
    {
        r /= rhs.r;
        g /= rhs.g;
        b /= rhs.b;
        a /= rhs.a;
        return *this;
    }

    Color &operator/=(float rhs)
    {
        r /= rhs;
        g /= rhs;
        b /= rhs;
        a /= rhs;
        return *this;
    }

    bool operator==(const Color &other) const
    {
        return (r == other.r && g == other.g && b == other.b && a == other.a);
    }

    bool operator!=(const Color &other) const
    {
        return (r != other.r || g != other.g || b != other.b || a != other.a);
    }

    Color operator+() const
    {
        return *this;
    }

    Color operator-() const
    {
        return Color(-r, -g, -b, -a);
    }

    friend Color operator+(const Color &lhs, const Color &rhs)
    {
        return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
    }

    friend Color operator-(const Color &lhs, const Color &rhs)
    {
        return Color(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a);
    }

    friend Color operator*(const Color &lhs, const Color &rhs)
    {
        return Color(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a);
    }

    friend Color operator*(float lhs, const Color &rhs)
    {
        return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b, lhs * rhs.a);
    }

    friend Color operator*(const Color &lhs, float rhs)
    {
        return Color(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs);
    }

    friend Color operator/(const Color &lhs, const Color &rhs)
    {
        return Color(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b, lhs.a / rhs.a);
    }

    friend Color operator/(float lhs, const Color &rhs)
    {
        return Color(lhs / rhs.r, lhs / rhs.g, lhs / rhs.b, lhs / rhs.a);
    }

    friend Color operator/(const Color &lhs, float rhs)
    {
        return Color(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a / rhs);
    }

    Color &Saturate()
    {
        r = Math::Clamp01(r);
        g = Math::Clamp01(g);
        b = Math::Clamp01(b);
        a = Math::Clamp01(a);
        return *this;
    }

    float Luminance() const
    {
        return Luminance(r, g, b);
    }

    static Color Lerp(const Color &a, const Color &b, float t)
    {
        return (b - a) * t + a;
    }

    static uint8 GetAlpha(const Color &color)
    {
        return static_cast<uint8>(color.a * 255);
    }

    static Color FromRGBA(uint32 value)
    {
        uint32 r = value >> 24;
        uint32 g = (value & 0x00ff0000) >> 16;
        uint32 b = (value & 0x0000ff00) >> 8;
        uint32 a = (value & 0x000000ff);
        return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    static uint32 ToRGBA(const Color &color)
    {
        uint32 r = static_cast<uint8>(color.r * 255);
        uint32 g = static_cast<uint8>(color.g * 255);
        uint32 b = static_cast<uint8>(color.b * 255);
        uint32 a = static_cast<uint8>(color.a * 255);
        return (r << 24) | (g << 16) | (b << 8) | (a << 0);
    }

    //Assume h, s, v is form 0 to 1.
    static Color FromHSV(float h, float s, float v)
    {
        if (v == 0.0f)
        {
            return Color(0.0f, 0.0f, 0.0f);
        }
        if (s == 0.0f)
        {
            return Color(v, v, v);
        }

        const float x = (h >= 1.0f ? 0.0f : h) * 6.0f;
        const int32 i = Math::TruncToInt(x);
        const float f = x - i;
        const float f1 = v * (1.0f - s);
        const float f2 = v * (1.0f - s * f);
        const float f3 = v * (1.0f - s * (1.0f - f));

        float r, g, b;
        switch (i)
        {
        case 0:
            r = v;
            g = f3;
            b = f1;
            break;
        case 1:
            r = f2;
            g = v;
            b = f1;
            break;
        case 2:
            r = f1;
            g = v;
            b = f3;
            break;
        case 3:
            r = f1;
            g = f2;
            b = v;
            break;
        case 4:
            r = f3;
            g = f1;
            b = v;
            break;
        default:
            r = v;
            g = f1;
            b = f2;
            break;
        }
        return Color(r, g, b);
    }

    static Vector3 ToHSV(const Color &color)
    {
        float h, s, v;

        const float minVal = Math::Min(color.r, color.g, color.b);
        const float maxVal = Math::Max(color.r, color.g, color.b);
        const float delta = maxVal - minVal;

        v = maxVal;

        if (delta < Math::EPSILON)
        {
            h = s = 0.0f;
        }
        else
        {
            s = delta / maxVal;

            if (maxVal == color.r)
                h = (color.g - color.b) / (6.0f * delta);
            else if (maxVal == color.g)
                h = 1.0f / 3.0f + (color.b - color.r) / (6.0f * delta);
            else
                h = 2.0f / 3.0f + (color.r - color.g) / (6.0f * delta);

            if (h < 0.0f)
                h += 1.0f;
            if (h > 1.0f)
                h -= 1.0f;
        }

        return Vector3(h, s, v);
    }

    static float Luminance(float r, float g, float b)
    {
        return 0.2126f * r + 0.7152f * g + 0.0722f * b;
    }

    String ToString() const
    {
        return String::Format(CT_TEXT("[r={0},g={1},b={2},a={3}]"), r, g, b, a);
    }

    uint32 HashCode() const
    {
        uint32 hash = Hash::HashValue(r);
        Hash::HashCombine(hash, Hash::HashValue(g));
        Hash::HashCombine(hash, Hash::HashValue(b));
        Hash::HashCombine(hash, Hash::HashValue(a));
        return hash;
    }
};

inline const Color Color::WHITE(1.0f, 1.0f, 1.0f);
inline const Color Color::BLACK(0.0f, 0.0f, 0.0f);
inline const Color Color::RED(1.0f, 0.0f, 0.0f);
inline const Color Color::GREEN(0.0f, 1.0f, 0.0f);
inline const Color Color::BLUE(0.0f, 0.0f, 1.0f);

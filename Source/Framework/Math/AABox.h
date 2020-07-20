#pragma once

#include "Math/Vector3.h"

class AABox
{
public:
    Vector3 min{ -0.5f, -0.5f, -0.5f };
    Vector3 max{ 0.5f, 0.5f, 0.5f };

    AABox() = default;
    AABox(const Vector3 &min, const Vector3 &max)
        : min(min), max(max)
    {
    }

    Vector3 GetMin() const
    {
        return min;
    }

    AABox &SetMin(const Vector3 &v)
    {
        min = v;
        return *this;
    }

    Vector3 GetMax() const
    {
        return max;
    }

    AABox &SetMax(const Vector3 &v)
    {
        max = v;
        return *this;
    }

    Vector3 GetCenter() const
    {
        return (min + max) * 0.5f;
    }

    Vector3 GetSize() const
    {
        return max - min;
    }

    bool operator=(const AABox &other) const
    {
        return min == other.min && max == other.max;
    }

    bool operator!=(const AABox &other) const
    {
        return min != other.min || max != other.max;
    }
};
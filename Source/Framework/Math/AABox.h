#pragma once

#include "Math/Matrix4.h"
#include "Math/Vector3.h"

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

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

    AABox &Transform(const Matrix4 &mat)
    {
        Vector3 v0 = Vector3(mat.GetColumn(0));
        Vector3 v1 = Vector3(mat.GetColumn(1));
        Vector3 v2 = Vector3(mat.GetColumn(2));
        Vector3 v3 = Vector3(mat.GetColumn(3));

        Vector3 xa = v0 * min.x;
        Vector3 xb = v0 * max.x;
        Vector3 xMin = Vector3::Min(xa, xb);
        Vector3 xMax = Vector3::Max(xa, xb);

        Vector3 ya = v1 * min.y;
        Vector3 yb = v1 * max.y;
        Vector3 yMin = Vector3::Min(ya, yb);
        Vector3 yMax = Vector3::Max(ya, yb);

        Vector3 za = v2 * min.z;
        Vector3 zb = v2 * max.z;
        Vector3 zMin = Vector3::Min(za, zb);
        Vector3 zMax = Vector3::Max(za, zb);

        min = xMin + yMin + zMin + v3;
        max = xMax + yMax + zMax + v3;
        return *this;
    }

    AABox &Merge(const AABox &other)
    {
        min = Vector3::Min(min, other.min);
        max = Vector3::Max(max, other.max);
        return *this;
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

    float GetRadius() const
    {
        return ((max - min) * 0.5f).Length();
    }

    float GetVolume() const
    {
        Vector3 s = max - min;
        return s.x * s.y * s.z;
    }

    static AABox Transform(const AABox &box, const Matrix4 &mat)
    {
        AABox result = box;
        return result.Transform(mat);
    }

    static AABox Merge(const AABox &lhs, const AABox &rhs)
    {
        AABox result = lhs;
        return result.Merge(rhs);
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

#pragma pop_macro("min")
#pragma pop_macro("max")
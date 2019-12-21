#pragma once

#include "Math/.Package.h"
#include "Math/Vector3.h"

class Sphere
{
public:
    Vector3 center;
    float radius = 0.0f;

    Sphere() = default;

    Sphere(const Vector3 &center, float radius) : center(center), radius(radius)
    {
    }

    float GetRadius() const
    {
        return radius;
    }

    Sphere &SetRadius(float r)
    {
        radius = r;
        return *this;
    }

    Vector3 GetCenter() const
    {
        return center;
    }

    Sphere &SetCenter(const Vector3 &c)
    {
        center = c;
        return *this;
    }

    bool Contains(const Vector3 &point) const
    {
        float dst2 = center.Distance2(point);
        return dst2 <= radius * radius;
    }

    bool Contains(const Sphere &other) const
    {
        float radiusDiff = radius - other.radius;
        if (radiusDiff < 0.0f)
            return false;

        float dst2 = center.Distance2(other.center);
        return dst2 <= radiusDiff * radiusDiff;
    }

    bool Overlaps(const Sphere &other) const
    {
        float dst2 = center.Distance2(other.center);
        float radiusSum = radius + other.radius;
        return dst2 < radiusSum * radiusSum;
    }

    bool operator=(const Sphere &other) const
    {
        return radius == other.radius && center == other.center;
    }

    bool operator!=(const Sphere &other) const
    {
        return radius != other.radius || center != other.center;
    }

    String ToString() const
    {
        return String::Format(CT_TEXT("[center={0},radius={1}]"), center, radius);
    }
};
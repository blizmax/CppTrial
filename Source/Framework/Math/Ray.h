#pragma once

#include "Math/.Package.h"
#include "Math/Matrix4.h"
#include "Math/Vector3.h"

//The direction must be normalized.
class Ray
{
public:
    Vector3 origin;
    Vector3 direction = Vector3::Z;

    Ray() = default;

    Ray(const Vector3 &origin, const Vector3 &direction)
        : origin(origin), direction(direction)
    {
    }

    Ray &SetOrigin(const Vector3 &orig)
    {
        origin = orig;
        return *this;
    }

    Vector3 GetOrigin() const
    {
        return origin;
    }

    Ray &SetDirection(const Vector3 &dir)
    {
        direction = dir;
        return *this;
    }

    Vector3 GetDirection() const
    {
        return direction;
    }

    Vector3 GetPoint(float distance) const
    {
        return origin + distance * direction;
    }

    friend Ray operator*(const Ray &lhs, const Matrix4 &rhs)
    {
        Vector3 origin = rhs * lhs.origin;
        Vector3 temp = rhs * lhs.GetPoint(1.0f) - origin;
        temp.Normalize();
        return Ray(origin, temp);
    }

    String ToString() const
    {
        return String::Format(CT_TEXT("[origin={0},direction={1}]"), origin, direction);
    }
};
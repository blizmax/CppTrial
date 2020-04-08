#pragma once

#include "Math/Vector3.h"
#include "Math/Quat.h"

class Transform
{
public:
    Transform() = default;
    Transform(const Vector3 &position, const Quat &rotation, const Vector3 &scale)
        : position(position), rotation(rotation), scale(scale)
    {
    }

    Transform &SetPosition(const Vector3 &newPosition)
    {
        position = newPosition;
        return *this;
    }

    Vector3 GetPosition() const
    {
        return position;
    }

    Transform &SetRotation(const Quat &newRotation)
    {
        rotation = newRotation;
        return *this;
    }

    Quat GetRotation() const
    {
        return rotation;
    }

    Transform &SetScale(const Vector3 &newScale)
    {
        scale = newScale;
        return *this;
    }

    Vector3 GetScale() const
    {
        return scale;
    }

    Vector3 GetForward() const;
    Vector3 GetUp() const;
    Vector3 GetRight() const;

    Transform &Translate(const Vector3 &vec);
    Transform &Rotate(const Quat &quat); //Assume quat is normalized.
    Transform &LookAt(const Vector3 &target, const Vector3 &up = Vector3::Y);

    Matrix4 ToMatrix4() const;

private:
    Quat rotation;
    Vector3 position;
    Vector3 scale = Vector3::ONE;
};
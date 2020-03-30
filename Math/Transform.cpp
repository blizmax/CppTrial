#include "Math/Transform.h"
#include "Math/Matrix4.h"

Vector3 Transform::GetForward() const
{
    return GetRotation().Rotate(Vector3::Z);
}

Vector3 Transform::GetUp() const
{
    return GetRotation().Rotate(Vector3::Y);
}

Vector3 Transform::GetRight() const
{
    return GetRotation().Rotate(Vector3::X);
}

Transform &Transform::LookAt(const Vector3 &target, const Vector3 &up)
{
    Vector3 forward = target - GetPosition();
    Quat rot = GetRotation();
    rot.SetLookRotation(forward, up);
    SetRotation(rot);
    return *this;
}

Transform &Transform::Translate(const Vector3 &vec)
{
    SetPosition(GetPosition() + vec);
    return *this;
}

//Assume quat is normalized.
Transform &Transform::Rotate(const Quat &quat)
{
    SetRotation(quat * GetRotation());
    return *this;
}

Matrix4 Transform::ToMatrix4() const
{
    return Matrix4::TRS(position, rotation, scale);
}

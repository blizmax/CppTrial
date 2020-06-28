#include "Render/Camera.h"

SPtr<Camera> Camera::Create()
{
    return Memory::MakeShared<Camera>();
}

void Camera::SetPosition(const Vector3 &pos)
{
    data.pos = pos;
    dirty = true;
}

void Camera::SetUp(const Vector3 &up)
{
    data.up = up;
    dirty = true;
}

void Camera::SetTarget(const Vector3 &target)
{
    data.target = target;
    dirty = true;
}

void Camera::SetFocalLength(float value)
{
    data.focalLength = value;
    dirty = true;
}

void Camera::SetAspectRatio(float value)
{
    data.aspectRatio = value;
    dirty = true;
}

void Camera::SetNearZ(float value)
{
    data.nearZ = value;
    dirty = true;
}

void Camera::SetFarZ(float value)
{
    data.farZ = value;
    dirty = true;
}
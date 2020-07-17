#include "Render/Camera.h"

static float FocalLengthToFov(float focalLength, float frameHeight)
{
    return 2.0f * Math::Atan(0.5f * frameHeight / focalLength);
}

static float FovToFocalLength(float fov, float frameHeight)
{
    return frameHeight / (2.0f * tan(0.5f * fov));
}

SPtr<Camera> Camera::Create()
{
    return Memory::MakeShared<Camera>();
}

void Camera::CalculateParameters() const
{
    if (!dirty)
        return;

    dirty = false;

    const float fovY = data.focalLength == 0.0f ? 0.0f : FocalLengthToFov(data.focalLength, data.frameHeight);

    data.viewMat = Matrix4::LookAt(data.posW, data.target, data.up);
    if (fovY != 0.0f)
    {
        data.projMat = Matrix4::Projection(fovY, data.aspectRatio, data.nearZ, data.farZ);
    }
    else
    {
        const float h = (data.posW - data.target).Length() * 0.5f;
        data.projMat = Matrix4::Ortho(-h, h, -h, h, data.nearZ, data.farZ);
    }

    data.viewProjMat = data.projMat * data.viewMat;
    data.invViewProj = data.viewProjMat.Inverse();

    //TODO jitter mat, frustum
}

const Matrix4 &Camera::GetView() const
{
    CalculateParameters();
    return data.viewMat;
}

const Matrix4 &Camera::GetProjection() const
{
    CalculateParameters();
    return data.projMat;
}

const Matrix4 &Camera::GetViewProjection() const
{
    CalculateParameters();
    return data.viewProjMat;
}

const Matrix4 &Camera::GetInvViewProjection() const
{
    CalculateParameters();
    return data.invViewProj;
}
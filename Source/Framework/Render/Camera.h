#pragma once

#include "Render/.Package.h"

class Camera
{
public:
    virtual ~Camera() = default;

    bool Update();
    const Matrix4 &GetView() const;
    const Matrix4 &GetProjection() const;
    const Matrix4 &GetViewProjection() const;
    const Matrix4 &GetInvViewProjection() const;
    const CameraData &GetData() const;

    void SetPosition(const Vector3 &pos)
    {
        data.posW = pos;
        MarkDirty();
    }

    Vector3 GetPosition() const
    {
        return data.posW;
    }

    void SetUp(const Vector3 &up)
    {
        data.up = up;
        MarkDirty();
    }

    Vector3 GetUp() const
    {
        return data.up;
    }

    void SetTarget(const Vector3 &target)
    {
        data.target = target;
        MarkDirty();
    }

    Vector3 GetTarget() const
    {
        return data.target;
    }

    void SetFocalLength(float value)
    {
        data.focalLength = value;
        MarkDirty();
    }

    float GetFocalLength() const
    {
        return data.focalLength;
    }

    void SetAspectRatio(float value)
    {
        data.aspectRatio = value;
        MarkDirty();
    }

    float GetAspectRatio() const
    {
        return data.aspectRatio;
    }

    void SetNearZ(float value)
    {
        data.nearZ = value;
        MarkDirty();
    }

    float GetNearZ() const
    {
        return data.nearZ;
    }

    void SetFarZ(float value)
    {
        data.farZ = value;
        MarkDirty();
    }

    float GetFarZ() const
    {
        return data.farZ;
    }

    void SetFrameHeight(float value)
    {
        data.frameHeight = value;
        MarkDirty();
    }

    float GetFrameHeight() const
    {
        return data.frameHeight;
    }

    void SetFocalDistance(float value)
    {
        data.focalDistance = value;
        MarkDirty();
    }

    float GetFocalDistance() const
    {
        return data.focalDistance;
    }

    void SetApertureRadius(float value)
    {
        data.apertureRadius = value;
        MarkDirty();
    }

    float GetApertureRadius() const
    {
        return data.apertureRadius;
    }

    void SetShutterSpeed(float value)
    {
        data.shutterSpeed = value;
        MarkDirty();
    }

    float GetShutterSpeed() const
    {
        return data.shutterSpeed;
    }

    const String &GetName() const
    {
        return name;
    }

    void SetName(const String &newName)
    {
        name = newName;
    }

    static SPtr<Camera> Create();

private:
    void MarkDirty();
    void CalculateParameters() const;

private:
    String name;
    mutable CameraData data{};
    mutable bool dirty = true;
    bool changedSinceLastUpdate = false;
};
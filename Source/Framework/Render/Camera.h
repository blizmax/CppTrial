#pragma once

#include "Render/.Package.h"

struct CameraData
{
    Matrix4 viewMat;
    Matrix4 projMat;
    Matrix4 viewProjMat;
    Matrix4 invViewProj;

    Vector3 pos; // world space position
    float focalLength = 21.0f;
    Vector3 up{0.0f, 1.0f, 0.0f};
    float aspectRatio = 1.7777f; // 16:9
    Vector3 target{0.0f, 0.0f, -1.0f};
    float nearZ = 0.1f;
    Vector3 cameraU{0.0f, 0.0f, 1.0f};
    float farZ = 1000.0f;
    Vector3 cameraV{0.0f, 1.0f, 0.0f};
    float jitterX = 0.0f;
    Vector3 cameraW{1.0f, 0.0f, 0.0f};
    float jitterY = 0.0f;
};

class Camera
{
public:
    virtual ~Camera() = default;

    void SetPosition(const Vector3 &pos);
    void SetUp(const Vector3 &up);
    void SetTarget(const Vector3 &target);
    void SetFocalLength(float value);
    void SetAspectRatio(float value);
    void SetNearZ(float value);
    void SetFarZ(float value);

    Vector3 GetPosition() const
    {
        return data.pos;
    }

    Vector3 GetUp() const
    {
        return data.up;
    }

    Vector3 GetTarget() const
    {
        return data.target;
    }

    float GetFocalLength() const
    {
        return data.focalLength;
    }

    float GetAspectRatio() const
    {
        return data.aspectRatio;
    }

    float GetNearZ() const
    {
        return data.nearZ;
    }

    float GetFarZ() const
    {
        return data.farZ;
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
    String name;
    CameraData data{};
    bool dirty = true;
};
#pragma once

#include "Render/.Package.h"
#include "Math/Matrix4.h"
#include "Math/Vector3.h"

struct CameraData
{
    Matrix4 viewMat;
    Matrix4 projMat;
    Matrix4 viewProjMat;
    Matrix4 invViewProj;

    Vector3 posW;
    float focalLength = 21.0f;
    Vector3 up {0.0f, 1.0f, 0.0f};
    float aspectRatio = 1.7777f; // 16:9
    Vector3 target {0.0f, 0.0f, -1.0f};
    float nearZ = 0.1f;
    Vector3 cameraU {0.0f, 0.0f, 1.0f};
    float farZ = 1000.0f;
    Vector3 cameraV {0.0f, 1.0f, 0.0f};
    float jitterX = 0.0f;
    Vector3 cameraW {1.0f, 0.0f, 0.0f};
    float jitterY = 0.0f;
};

class Camera
{
public:
    virtual ~Camera() = default;

    static SPtr<Camera> Create();

private:
};
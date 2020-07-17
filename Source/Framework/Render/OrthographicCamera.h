#pragma once

#include "Math/Matrix4.h"
#include "Math/Vector3.h"
#include "Render/.Package.h"

class OrthographicCamera
{
public:
    Vector3 position;
    Vector3 direction{ 0.0f, 0.0f, -1.0f };
    Vector3 up{ 0.0f, 1.0f, 0.0f };
    Vector3 right{ 1.0f, 0.0f, 0.0f };

    Matrix4 projection;
    Matrix4 view;
    Matrix4 combined;

    float viewportWidth;
    float viewportHeight;

    float near = -1.0f;
    float far = 1.0f;
    float zoom = 1.0f;

public:
    explicit OrthographicCamera(float viewportWidth, float viewportHeight)
        : viewportWidth(viewportWidth), viewportHeight(viewportHeight)
    {
        Update();
    }

    void Update();
    void Normalize();

    static SPtr<OrthographicCamera> Create(float viewportWidth, float viewportHeight)
    {
        return Memory::MakeShared<OrthographicCamera>(viewportWidth, viewportHeight);
    }
};
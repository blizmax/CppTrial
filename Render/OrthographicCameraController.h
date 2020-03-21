#pragma once

#include "Render/OrthographicCamera.h"
#include "Application/Input.h"

class OrthographicCameraController
{
public:
    OrthographicCameraController() = default;
    OrthographicCameraController(SPtr<OrthographicCamera> camera);

    void Update();
    void OnTouchDown();
    void OnTouchUp();
    void OnKeyDown();
    void OnKeyUp();
    void OnMouseScrolled();
    void OnWindowResized();

    void SetCamera(SPtr<OrthographicCamera> newCamera)
    {
        camera = newCamera;
    }

    SPtr<OrthographicCamera> GetCamera() const
    {
        return camera;
    }

private:
    SPtr<OrthographicCamera> camera;
};
#pragma once

#include "Render/Camera.h"
#include "Application/InputEvent.h"

class CameraController
{
public:
    virtual ~CameraController() = default;

    virtual void Update() = 0;

    virtual void OnTouchDown(TouchDownEvent &event) {}
    virtual void OnTouchUp(TouchUpEvent &event) {}
    virtual void OnMouseMoved(MouseMovedEvent &event) {}
    virtual void OnMouseScrolled(MouseScrolledEvent &event) {}
    virtual void OnKeyDown(KeyDownEvent &event) {}
    virtual void OnKeyUp(KeyUpEvent &event) {}

    void SetCameraSpeed(float newSpeed)
    {
        speed = newSpeed;
    }

protected:
    CameraController(const SPtr<Camera> &camera);

protected:
    SPtr<Camera> camera;
    float speed = 1.0f;
};
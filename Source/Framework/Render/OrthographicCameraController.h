#pragma once

#include "Render/OrthographicCamera.h"
#include "Application/InputEvent.h"
#include "Application/WindowEvent.h"
#include "Math/Vector2.h"

class OrthographicCameraController
{
public:
    OrthographicCameraController() = default;
    OrthographicCameraController(SPtr<OrthographicCamera> camera) : camera(camera)
    {
    }

    void Update();

    void OnTouchDown(TouchDownEvent &event);
    void OnTouchUp(TouchUpEvent &event);
    void OnMouseMoved(MouseMovedEvent &event);
    void OnMouseScrolled(MouseScrolledEvent &event);
    void OnKeyDown(KeyDownEvent &event);
    void OnKeyUp(KeyUpEvent &event);
    void OnWindowResized(WindowResizedEvent &event);

    void SetCamera(SPtr<OrthographicCamera> newCamera)
    {
        camera = newCamera;
    }

    SPtr<OrthographicCamera> GetCamera() const
    {
        return camera;
    }
protected:
    void ProcessDrag(int32 deltaX, int32 deltaY, int32 button);
    void ProcessZoom(int32 amount);

protected:
    SPtr<OrthographicCamera> camera;
    bool dragging = false;
    int32 button;
    int32 startX;
    int32 startY;
};
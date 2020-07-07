#pragma once

#include "Render/Camera.h"
#include "Application/InputEvent.h"

class CameraController
{
public:
    CameraController(const SPtr<Camera> &camera) : camera(camera)
    {
    }

    virtual ~CameraController() = default;

    virtual bool Update() = 0;
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

    void SetViewport(float w, float h)
    {
        viewportWidth = w;
        viewportHeight = h;
    }

protected:
    SPtr<Camera> camera;
    float speed = 1.0f;
    float viewportWidth = 1.0f;
    float viewportHeight = 1.0f;
};

class OrbiterCameraController : public CameraController
{
public:
    OrbiterCameraController(const SPtr<Camera> &camera) : CameraController(camera)
    {
    }

    virtual bool Update() override;
    virtual void OnTouchDown(TouchDownEvent &event) override;
    virtual void OnTouchUp(TouchUpEvent &event) override;
    virtual void OnMouseMoved(MouseMovedEvent &event) override;
    virtual void OnMouseScrolled(MouseScrolledEvent &event) override;

    static SPtr<OrbiterCameraController> Create(const SPtr<Camera> &camera)
    {
        return Memory::MakeShared<OrbiterCameraController>(camera);
    }

private:
    Vector3 modelCenter;
    float modelRadius;
    float cameraDistance;
    Quat rotation;
    Vector3 lastVector;
    bool dragging = false;

    bool dirty = false;
};
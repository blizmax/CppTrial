#include "Render/OrthographicCameraController.h"

void OrthographicCameraController::Update()
{
    if(camera)
    {
        camera->Update();
    }
}

void OrthographicCameraController::OnTouchDown(TouchDownEvent &event)
{
    if(!dragging && event.button == CT_BUTTON_MIDDLE)
    {
        dragging = true;
        button = event.button;
        startX = event.x;
        startY = event.y;
    }
}

void OrthographicCameraController::OnTouchUp(TouchUpEvent &event)
{
    if(event.button == CT_BUTTON_MIDDLE)
    {
        dragging = false;
    }
}

void OrthographicCameraController::OnMouseMoved(MouseMovedEvent &event)
{
    if(dragging)
    {
        int32 deltaX = event.x - startX;
        int32 deltaY = event.y - startY;
        startX = event.x;
        startY = event.y;

        ProcessDrag(deltaX, deltaY, button);
    }
}

void OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent &event)
{
    ProcessZoom(event.amount);
}

void OrthographicCameraController::OnKeyDown(KeyDownEvent &event)
{

}

void OrthographicCameraController::OnKeyUp(KeyUpEvent &event)
{
    if(event.key == CT_KEY_R)
    {
        camera->zoom = 1.0f;
        camera->position = {0.0f, 0.0f, 0.0f};
    }
}

void OrthographicCameraController::OnWindowResized(WindowResizedEvent &event)
{
    if(camera)
    {
        camera->viewportWidth = (float)event.width;
        camera->viewportHeight = (float)event.height;
    }
}

void OrthographicCameraController::ProcessDrag(int32 x, int32 y, int32 button)
{
    if(!camera)
        return;

    camera->position.x -= x * camera->zoom;
    camera->position.y += y * camera->zoom;
}

void OrthographicCameraController::ProcessZoom(int32 amount)
{
    if(!camera)
        return;

    const float zoomFactor = -0.1f;
    float zoom = Math::Clamp(camera->zoom + zoomFactor * amount, 0.25f, 10.0f);
    camera->zoom = zoom;
}
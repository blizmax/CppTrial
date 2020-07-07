#include "Render/CameraController.h"

static Vector3 Project2DToUnitSphere(Vector2 v)
{
    float dist2 = v.Length2();
    float z = 0.0f;

    if(dist2 < 1.0f)
    {
        z = Math::Sqrt(1.0f - dist2);
    }
    else
    {
        v.Normalize();
    }
    return float3(v.x, v.y, z);
}

static Vector2 ConvertScreenPosition(int32 x, int32 y, float viewportW, float viewportH)
{
    x /= viewportW;
    y /= viewportH;
    return Vector2(x * 2.0f - 1.0f, -(y * 2.0f - 1.0f));
}

bool OrbiterCameraController::Update()
{
    if (camera && dirty)
    {
        dirty = false;

        camera->SetTarget(modelCenter);
        
        Vector3 camPos = modelCenter;
        camPos += rotation.Rotate(Vector3::Z) * modelRadius * cameraDistance;
        camera->SetPosition(camPos);

        Vector3 camUp = rotation.Rotate(Vector3::Y);
        camera->SetUp(camUp);
    
        return true;
    }
    return false;
}

void OrbiterCameraController::OnTouchDown(TouchDownEvent &event)
{

}

void OrbiterCameraController::OnTouchUp(TouchUpEvent &event)
{

}

void OrbiterCameraController::OnMouseMoved(MouseMovedEvent &event)
{
    if (!dragging)
        return;

    Vector3 curVector = Project2DToUnitSphere(ConvertScreenPosition(event.x, event.y, viewportWidth, viewportHeight));
    Quat quat;
    quat.SetFromToRotation(lastVector, curVector);
    rotation = quat * rotation;
    lastVector = curVector;

    dirty = true;
}

void OrbiterCameraController::OnMouseScrolled(MouseScrolledEvent &event)
{

}
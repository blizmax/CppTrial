#include "Render/CameraController.h"

static Vector3 Project2DToUnitSphere(Vector2 v)
{
    float dist2 = v.Length2();
    float z = 0.0f;

    if (dist2 < 1.0f)
    {
        z = Math::Sqrt(1.0f - dist2);
    }
    else
    {
        v.Normalize();
    }
    return Vector3(v.x, v.y, z);
}

static Vector2 ConvertScreenPosition(int32 x, int32 y, float viewportW, float viewportH)
{
    x /= viewportW;
    y /= viewportH;
    return Vector2(x * 2.0f - 1.0f, -(y * 2.0f - 1.0f));
}

void OrbiterCameraController::SetModelParams(const Vector3 &center, float radius, float distance)
{
    modelCenter = center;
    modelRadius = radius;
    cameraDistance = distance;
    rotation.SetIndentity();

    dirty = true;
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
    if (event.button != CT_BUTTON_LEFT)
        return;

    dragging = true;
    lastVector = Project2DToUnitSphere(ConvertScreenPosition(event.x, event.y, viewportWidth, viewportHeight));
}

void OrbiterCameraController::OnTouchUp(TouchUpEvent &event)
{
    if (event.button != CT_BUTTON_LEFT)
        return;

    dragging = false;
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
    cameraDistance -= event.amount * 0.2f;

    dirty = true;
}

template <bool b6DOF>
bool TFirstPersonCameraController<b6DOF>::Update()
{
    bool updated = false;

    if (camera)
    {
        if (shouldRotate)
        {
            shouldRotate = false;

            Vector3 camPos = camera->GetPosition();
            Vector3 camTarget = camera->GetTarget();
            Vector3 camUp = b6DOF ? camera->GetUp() : Vector3::Y;

            Vector3 viewDir = camTarget - camPos;
            viewDir.Normalize();

            if (leftBtnDown)
            {
                Vector3 sideway = viewDir.Cross(camUp.Normalize());

                Quat qx(sideway, delta.y);
                viewDir = qx.Rotate(viewDir);
                camUp = qx.Rotate(camUp);

                Quat qy(camUp, delta.x);
                viewDir = qy.Rotate(viewDir);

                camera->SetTarget(camPos + viewDir);
                camera->SetUp(camUp);

                updated = true;
            }

            if (rightBtnDown && b6DOF)
            {
                Quat qz(viewDir, delta.x);
                camUp = qz.Rotate(camUp);

                camera->SetUp(camUp);

                updated = true;
            }
        }

        Vector3 movement;
        if (keyPressed[DIR_FORWARD])
        {
            movement.z += 1.0;
            updated = true;
        }
        if (keyPressed[DIR_BACKWARD])
        {
            movement.z -= 1.0;
            updated = true;
        }
        if (keyPressed[DIR_RIGHT])
        {
            movement.x += 1.0;
            updated = true;
        }
        if (keyPressed[DIR_LEFT])
        {
            movement.x -= 1.0;
            updated = true;
        }
        if (keyPressed[DIR_UP])
        {
            movement.y += 1.0;
            updated = true;
        }
        if (keyPressed[DIR_DOWN])
        {
            movement.y -= 1.0;
            updated = true;
        }

        Vector3 camPos = camera->GetPosition();
        Vector3 camTarget = camera->GetTarget();
        Vector3 camUp = camera->GetUp();

        Vector3 viewDir = camTarget - camPos;
        viewDir.Normalize();
        Vector3 sideway = viewDir.Cross(camUp.Normalize());

        camPos += movement.z * viewDir * speed;
        camPos += movement.x * sideway * speed;
        camPos += movement.y * camUp * speed;

        camera->SetPosition(camPos);
        camera->SetTarget(camPos + viewDir);
    }

    return updated;
}

template <bool b6DOF>
void TFirstPersonCameraController<b6DOF>::OnTouchDown(TouchDownEvent &event)
{
    switch (event.button)
    {
    case CT_BUTTON_LEFT:
        leftBtnDown = true;
        lastPos = ConvertScreenPosition(event.x, event.y, viewportWidth, viewportHeight);
        break;
    case CT_BUTTON_RIGHT:
        rightBtnDown = true;
        lastPos = ConvertScreenPosition(event.x, event.y, viewportWidth, viewportHeight);
        break;
    default:
        break;
    }
}

template <bool b6DOF>
void TFirstPersonCameraController<b6DOF>::OnTouchUp(TouchUpEvent &event)
{
    switch (event.button)
    {
    case CT_BUTTON_LEFT:
        leftBtnDown = false;
        break;
    case CT_BUTTON_RIGHT:
        rightBtnDown = false;
        break;
    default:
        break;
    }
}

template <bool b6DOF>
void TFirstPersonCameraController<b6DOF>::OnMouseMoved(MouseMovedEvent &event)
{
    if (leftBtnDown || rightBtnDown)
    {
        Vector2 pos = ConvertScreenPosition(event.x, event.y, viewportWidth, viewportHeight);
        delta = pos - lastPos;
        lastPos = pos;
        shouldRotate = true;
    }
}

template <bool b6DOF>
void TFirstPersonCameraController<b6DOF>::OnMouseScrolled(MouseScrolledEvent &event)
{
    // Do nothing
}

template <bool b6DOF>
void TFirstPersonCameraController<b6DOF>::OnKeyDown(KeyDownEvent &event)
{
    switch (event.key)
    {
    case CT_KEY_W:
        keyPressed[DIR_FORWARD] = true;
        break;
    case CT_KEY_S:
        keyPressed[DIR_BACKWARD] = true;
        break;
    case CT_KEY_A:
        keyPressed[DIR_LEFT] = true;
        break;
    case CT_KEY_D:
        keyPressed[DIR_RIGHT] = true;
        break;
    case CT_KEY_Q:
        keyPressed[DIR_UP] = true;
        break;
    case CT_KEY_E:
        keyPressed[DIR_DOWN] = true;
        break;
    default:
        break;
    }
}

template <bool b6DOF>
void TFirstPersonCameraController<b6DOF>::OnKeyUp(KeyUpEvent &event)
{
    switch (event.key)
    {
    case CT_KEY_W:
        keyPressed[DIR_FORWARD] = false;
        break;
    case CT_KEY_S:
        keyPressed[DIR_BACKWARD] = false;
        break;
    case CT_KEY_A:
        keyPressed[DIR_LEFT] = false;
        break;
    case CT_KEY_D:
        keyPressed[DIR_RIGHT] = false;
        break;
    case CT_KEY_Q:
        keyPressed[DIR_UP] = false;
        break;
    case CT_KEY_E:
        keyPressed[DIR_DOWN] = false;
        break;
    default:
        break;
    }
}

template class TFirstPersonCameraController<false>;
template class TFirstPersonCameraController<true>;

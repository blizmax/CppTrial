#include "Render/OrthographicCamera.h"

void OrthographicCamera::Update()
{
    projection = Matrix4::Ortho(zoom * -viewportWidth / 2, zoom * (viewportWidth / 2),
                                zoom * -(viewportHeight / 2), zoom * viewportHeight / 2, nearZ, farZ);

    view = Matrix4::LookAt(position, position + direction, up);
    combined = projection * view;
}

void OrthographicCamera::Normalize()
{
    direction.Normalize();
    right = direction.Cross(up).Normalize();
    up = right.Cross(direction).Normalize();
}
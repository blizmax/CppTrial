#include "Render/Light.h"

void Light::SetIntensity(const Color &color)
{
    data.intensity = Vector3(color.r, color.g, color.g);
}

SPtr<DirectionalLight> DirectionalLight::Create()
{
    return Memory::MakeShared<DirectionalLight>();
}

DirectionalLight::DirectionalLight()
{
    data.type = (int32)LightType::Directional;
}

void DirectionalLight::SetDirection(const Vector3 &dir)
{
    if (dir.Length2() > Math::EPSILON)
    {
        data.dirW = dir;
        data.dirW.Normalize();
        data.posW = center - data.dirW * distance;
    }
}

void DirectionalLight::SetCenter(const Vector3 &val)
{
    center = val;
    data.posW = center - data.dirW * distance;
}

void DirectionalLight::SetDistance(float val)
{
    distance = val;
    data.posW = center - data.dirW * distance;
}

float DirectionalLight::GetPower() const
{
    float surfaceArea = Math::PI * distance * distance;
    return Color::Luminance(data.intensity.x, data.intensity.y, data.intensity.z) * surfaceArea;
}

SPtr<PointLight> PointLight::Create()
{
    return Memory::MakeShared<PointLight>();
}

PointLight::PointLight()
{
    data.type = (int32)LightType::Point;
}

void PointLight::SetPosition(const Vector3 &pos)
{
    data.posW = pos;
}

void PointLight::SetDirection(const Vector3 &dir)
{
    if (dir.Length2() > Math::EPSILON)
    {
        data.dirW = dir;
        data.dirW.Normalize();
    }
}

void PointLight::SetOpeningAngle(float angle)
{
    angle = Math::Clamp(angle, 0.0f, Math::PI);
    data.openingAngle = angle;
    data.cosOpeningAngle = Math::Cos(angle);
}

void PointLight::SetPenumbraAngle(float angle)
{
    angle = Math::Clamp(angle, 0.0f, data.openingAngle);
    data.penumbraAngle = angle;
}

float PointLight::GetPower() const
{
    return Color::Luminance(data.intensity.x, data.intensity.y, data.intensity.z) * 4.0f * Math::PI;
}
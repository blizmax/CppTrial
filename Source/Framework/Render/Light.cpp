#include "Render/Light.h"

void Light::MarkDirty()
{
    dirty = true;
    changedSinceLastUpdate = true;
}

bool Light::Update()
{
    bool changed = changedSinceLastUpdate;
    changedSinceLastUpdate = false;

    return changed;
}

void Light::SetIntensity(const Vector3 &val)
{
    data.intensity = val;
    MarkDirty();
}

void Light::UpdateIntensityCache() const
{
    Vector3 cached = Vector3(intensityColor.r, intensityColor.g, intensityColor.b) * intensityScale;
    if (cached == data.intensity)
        return;

    float scl = Math::Max(data.intensity.x, data.intensity.y, data.intensity.z);
    if (scl < Math::EPSILON)
    {
        intensityColor = Color::WHITE;
        intensityScale = 0.0f;
    }
    else if (scl <= 1.0f)
    {
        intensityColor = Color(data.intensity.x, data.intensity.y, data.intensity.z);
        intensityScale = 1.0f;
    }
    else
    {
        intensityColor = Color(data.intensity.x / scl, data.intensity.y / scl, data.intensity.z / scl);
        intensityScale = scl;
    }
}

float Light::GetIntensityScale() const
{
    UpdateIntensityCache();
    return intensityScale;
}

Color Light::GetIntensityColor() const
{
    UpdateIntensityCache();
    return intensityColor;
}

void Light::SetIntensityColor(const Color &color)
{
    intensityColor = color;
    SetIntensity(Vector3(color.r, color.g, color.b) * intensityScale);
}

void Light::SetIntensityScale(float scl)
{
    intensityScale = scl;
    SetIntensity(Vector3(intensityColor.r, intensityColor.g, intensityColor.b) * scl);
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
        MarkDirty();
    }
}

void DirectionalLight::SetCenter(const Vector3 &val)
{
    center = val;
    data.posW = center - data.dirW * distance;
    MarkDirty();
}

void DirectionalLight::SetDistance(float val)
{
    distance = val;
    data.posW = center - data.dirW * distance;
    MarkDirty();
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
    MarkDirty();
}

void PointLight::SetDirection(const Vector3 &dir)
{
    if (dir.Length2() > Math::EPSILON)
    {
        data.dirW = dir;
        data.dirW.Normalize();
        MarkDirty();
    }
}

void PointLight::SetOpeningAngle(float angle)
{
    angle = Math::Clamp(angle, 0.0f, Math::PI);
    data.openingAngle = angle;
    data.cosOpeningAngle = Math::Cos(angle);
    MarkDirty();
}

void PointLight::SetPenumbraAngle(float angle)
{
    angle = Math::Clamp(angle, 0.0f, data.openingAngle);
    data.penumbraAngle = angle;
    MarkDirty();
}

float PointLight::GetPower() const
{
    return Color::Luminance(data.intensity.x, data.intensity.y, data.intensity.z) * 4.0f * Math::PI;
}
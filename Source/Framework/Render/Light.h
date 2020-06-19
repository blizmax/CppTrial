#pragma once

#include "Render/.Package.h"
#include "Math/Vector3.h"
#include "Core/Math.h"

enum class LightType
{
    Point,
    Directional,
};

struct LightData
{
    Vector3 position;
    int32 type = (int32)LightType::Point;
    Vector3 direction;
    float openingAngle = Math::PI;
    Vector3 intensity;
    float cosOpeningAngle = -1.0f;
};

class Light
{
public:
    virtual ~Light() = default;

    virtual void SetIntensity(const Color &color);

    LightType GetLightType() const
    {
        return (LightType)data.type;
    }

    const LightData &GetData() const
    {
        return data;
    }

protected:
    LightData data;
};

class DirectionalLight : public Light
{
public:

    static SPtr<DirectionalLight> Create();

private:
    float distance = 1e3f;
    Vector3 center;
};

class PointLight : public Light
{
public:

    static SPtr<PointLight> Create();
};
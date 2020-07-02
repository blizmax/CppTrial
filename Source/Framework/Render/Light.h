#pragma once

#include "Render/.Package.h"

enum class LightType
{
    Point,
    Directional,
};

struct LightData
{
    Vector3 pos{0.0f, 0.0f, 0.0f}; // world space position
    int32 type = (int32)LightType::Point;
    Vector3 dir{0.0f, -1.0f, 0.0f}; // world space direction
    float openingAngle = Math::PI;
    Vector3 intensity{1.0f, 1.0f, 1.0f};
    float cosOpeningAngle = -1.0f;
    float penumbraAngle = 0.0f;
    Vector3 _pad;
};

class Light
{
public:
    virtual ~Light() = default;

    virtual void SetIntensity(const Color &color);
    virtual float GetPower() const = 0;

    LightType GetLightType() const
    {
        return (LightType)data.type;
    }

    const LightData &GetData() const
    {
        return data;
    }

    const String &GetName() const
    {
        return name;
    }

    void SetName(const String &newName)
    {
        name = newName;
    }

protected:
    String name;
    LightData data{};
};

class DirectionalLight : public Light
{
public:
    DirectionalLight();

    void SetDirection(const Vector3 &dir);
    void SetCenter(const Vector3 &val);
    void SetDistance(float val);

    virtual float GetPower() const override;

    Vector3 GetDirection() const
    {
        return data.dir;
    }

    static SPtr<DirectionalLight> Create();

private:
    float distance = 1e3f;
    Vector3 center;
};

class PointLight : public Light
{
public:
    PointLight();

    void SetPosition(const Vector3 &pos);
    void SetDirection(const Vector3 &dir);
    void SetOpeningAngle(float angle);
    void SetPenumbraAngle(float angle);

    virtual float GetPower() const override;

    Vector3 GetPosition() const
    {
        return data.pos;
    }

    Vector3 GetDirection() const
    {
        return data.dir;
    }

    float GetOpeningAngle() const
    {
        return data.openingAngle;
    }

    Vector3 GetIntensity() const
    {
        return data.intensity;
    }

    static SPtr<PointLight> Create();
};
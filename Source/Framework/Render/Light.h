#pragma once

#include "Render/.Package.h"

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
        return data.dirW;
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
        return data.posW;
    }

    Vector3 GetDirection() const
    {
        return data.dirW;
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
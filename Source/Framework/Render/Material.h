#pragma once

#include "Render/.Package.h"
#include "RenderCore/Texture.h"

struct MaterialResources
{
    SPtr<Texture> colorTexture;
};

struct MaterialData
{
    Color baseColor;

};

class Material
{
public:
    Material(const String &name);

    const String &GetName() const
    {
        return name;
    }

    const MaterialData &GetData() const
    {
        return data;
    }

    const MaterialResources &GetResources() const
    {
        return resources;
    }

    static SPtr<Material> Create(const String &name);

private:
    String name;
    MaterialData data;
    MaterialResources resources;

};
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

    const MaterialData &GetData() const
    {
        return data;
    }

    const MaterialResources &GetResources() const
    {
        return resources;
    }

    static SPtr<Material> Create();

private:
    MaterialData data;
    MaterialResources resources;

};
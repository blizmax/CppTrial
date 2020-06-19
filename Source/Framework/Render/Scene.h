#pragma once

#include "Render/Material.h"
#include "Render/Light.h"

class Scene
{
public:
    int32 GetMaterialCount() const
    {
        return materials.Count();
    }

    const SPtr<Material> &GetMaterial(int32 index) const
    {
        return materials[index];
    }

    const Array<SPtr<Material>> &GetMaterials() const
    {
        return materials;
    }

    int32 GetLightCount() const
    {
        return lights.Count();
    }

    const SPtr<Light> &GetLight(int32 index) const
    {
        return lights[index];
    }

    const Array<SPtr<Light>> &GetLights() const
    {
        return lights;
    }

    static SPtr<Scene> Create();

private:
    Array<SPtr<Material>> materials;
    Array<SPtr<Light>> lights;
};
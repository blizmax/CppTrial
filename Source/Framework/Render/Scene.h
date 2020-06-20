#pragma once

#include "Render/CameraController.h"
#include "Render/Material.h"
#include "Render/Light.h"
#include "RenderCore/RenderAPI.h"

class Scene
{
public:
    void Render(RenderContext *ctx, GraphicsState *state, GraphicsVars *vars);

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
    SPtr<Camera> camera;
    SPtr<CameraController> cameraController;

    Array<SPtr<Material>> materials;
    Array<SPtr<Light>> lights;
};
#include "Render/SceneBuilder.h"

void SceneBuilder::SetCamera(const SPtr<Camera> &newCamera)
{
    camera = newCamera;
    dirty = true;
}

int32 SceneBuilder::AddLight(const SPtr<Light> &light)
{
    lights.Add(light);
    dirty = true;
    return lights.Count() - 1;
}

SPtr<Scene> GetScene()
{
    //TODO
    return nullptr;
}

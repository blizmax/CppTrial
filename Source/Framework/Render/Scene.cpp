#include "Render/Scene.h"

SPtr<Scene> Scene::Create()
{
    return Memory::MakeShared<Scene>();
}

void Scene::InitResources()
{
    //TODO
}

void Scene::Finalize()
{
    //TODO
}
#include "Render/Scene.h"

SPtr<Scene> Scene::Create()
{
    return Memory::MakeShared<Scene>();
}

void Scene::BindSamplerToMaterials(const SPtr<Sampler> &sampler)
{
    for (auto &m : materials)
    {
        m->SetSampler(sampler);
    }
}

void Scene::InitResources()
{
    //meshesBuffer = Buffer::CreateStructured();
}

void Scene::UploadResources()
{

}

void Scene::UploadMaterial(int32 matID)
{
    auto &mat = materials[matID];
    materialsBuffer->SetElement(matID, mat->GetData());

    const auto &resources = mat->GetResources();
    //TODO
}

void Scene::Finalize()
{
    //TODO
}
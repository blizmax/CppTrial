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

void Scene::SortMeshes()
{
    //TODO
}

void Scene::InitResources()
{
    meshesBuffer = Buffer::CreateStructured(sizeof(MeshDesc), meshDesces.Count(), ResourceBind::ShaderResource, CpuAccess::None, nullptr, false);
    meshInstancesBuffer = Buffer::CreateStructured(sizeof(MeshInstanceData), meshInstanceDatas.Count(), ResourceBind::ShaderResource, CpuAccess::None, nullptr, false);
    materialsBuffer = Buffer::CreateStructured(sizeof(MaterialData), materials.Count(), ResourceBind::ShaderResource, CpuAccess::None, nullptr, false);

    if (!lights.IsEmpty())
    {
        lightsBuffer = Buffer::CreateStructured(sizeof(LightData), lights.Count(), ResourceBind::ShaderResource, CpuAccess::None, nullptr, false);
    }
}

void Scene::UploadResources()
{
    meshesBuffer->SetBlob(meshDesces.GetData(), 0, meshDesces.Count() * sizeof(MeshDesc));
    meshInstancesBuffer->SetBlob(meshInstanceDatas.GetData(), 0, meshInstanceDatas.Count() * sizeof(MeshInstanceData));

    //TODO
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
    SortMeshes();
    InitResources();
    //TODO
}
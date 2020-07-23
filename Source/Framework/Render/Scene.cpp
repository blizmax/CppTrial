#include "Render/Scene.h"

namespace
{
const int32 PREV_VERTEX_BUFFER_INDEX = 2;

const String MESH_BUFFER_NAME = CT_TEXT("meshes");
const String MESH_INSTANCE_BUFFER_NAME = CT_TEXT("meshInstances");
const String PREV_VERTEX_BUFFER_NAME = CT_TEXT("prevVertexBuffer");
const String MATERIAL_BUFFER_NAME = CT_TEXT("materials");
const String LIGHT_BUFFER_NAME = CT_TEXT("lights");

}


SPtr<Scene> Scene::Create()
{
    return Memory::MakeShared<Scene>();
}

void Scene::Render(RenderContext *ctx, GraphicsState *state, GraphicsVars *vars)
{
    state->SetVertexArray(vao);
    vars->SetParameterBlock(sceneBlock);

    //TODO
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
    ProgramDefines defines;
    defines.Put(CT_TEXT("MATERIAL_COUNT"), StringConvert::ToString(materials.Count()));
    auto program = Program::Create(CT_TEXT("Assets/Shaders/Scene/SceneBlock.glsl"), defines);
    auto reflection = program->GetReflection();
    sceneBlock = ParameterBlock::Create(reflection->GetDefaultBlockReflection());

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

    sceneBlock->SetBuffer(MESH_BUFFER_NAME, meshesBuffer);
    sceneBlock->SetBuffer(MESH_INSTANCE_BUFFER_NAME, meshInstancesBuffer);
    sceneBlock->SetBuffer(PREV_VERTEX_BUFFER_NAME, vao->GetVertexBuffer(PREV_VERTEX_BUFFER_INDEX));

    sceneBlock->SetBuffer(MATERIAL_BUFFER_NAME, materialsBuffer);
    sceneBlock->SetBuffer(LIGHT_BUFFER_NAME, lightsBuffer);

    //TODO light probe
}

void Scene::UploadMaterial(int32 matID)
{
    auto &mat = materials[matID];
    materialsBuffer->SetElement(matID, mat->GetData());

    const auto &resources = mat->GetResources();
    auto var = sceneBlock->GetRootVar();

    auto SetTexture = [](const auto &var, const SPtr<Texture> &texture) {
        if (texture)
            var.SetTexture(texture);
        else
            var.SetTexture(Texture::GetDefault2D());
    };

    SetTexture(var[CT_TEXT("baseTextures")][matID], mat->GetBaseColorTexture());
    SetTexture(var[CT_TEXT("specularTextures")][matID], mat->GetSpecularTexture());
    SetTexture(var[CT_TEXT("emissiveTextures")][matID], mat->GetEmissiveTexture());
    SetTexture(var[CT_TEXT("normalTextures")][matID], mat->GetNormalTexture());
    SetTexture(var[CT_TEXT("occlusionTextures")][matID], mat->GetOcclusionTexture());

    var[CT_TEXT("samplerStates")][matID] = mat->GetSampler();
}


void Scene::UpdateBounds()
{
    auto &globalMatrices = animationController->GetGlobalMatrices();
    Array<AABox> instanceBBs;
    instanceBBs.Reserve(meshInstanceDatas.Count());

    for (const auto &e : meshInstanceDatas)
    {
        const auto &meshBB = meshBBs[e.meshID];
        const auto &transform = globalMatrices[e.globalMatrixID];
        instanceBBs.Add(AABox::Transform(meshBB, transform));
    }

    sceneBB = instanceBBs[0];
    for (int32 i = 1; i < instanceBBs.Count(); ++i)
    {
        sceneBB.Merge(instanceBBs[i]);
    }
}

void Scene::CreateDrawList()
{
    //TODO
}

void Scene::Finalize()
{
    SortMeshes();
    InitResources();
    //TODO
}
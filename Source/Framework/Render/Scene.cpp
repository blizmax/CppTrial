#include "Render/Scene.h"

namespace
{
const int32 STATIC_VERTEX_BUFFER_INDEX = 0;
const int32 PREV_VERTEX_BUFFER_INDEX = 2;

const String MESH_BUFFER_NAME = CT_TEXT("MeshDescBuffer");
const String MESH_INSTANCE_BUFFER_NAME = CT_TEXT("MeshInstanceBuffer");
const String STATIC_VERTEX_BUFFER_NAME = CT_TEXT("StaticVertexBuffer");
const String INDEX_BUFFER_NAME = CT_TEXT("IndexBuffer");
const String PREV_VERTEX_BUFFER_NAME = CT_TEXT("PrevVertexBuffer");
const String MATERIAL_BUFFER_NAME = CT_TEXT("MaterialBuffer");
const String LIGHT_BUFFER_NAME = CT_TEXT("LightBuffer");
const String CAMERA_BUFFER_NAME = CT_TEXT("CameraBuffer");

CT_INLINE bool DoesTransformFlip(const Matrix4 &m)
{
    return m.ToMatrix3().Determinant() < 0.0f;
}
}

SPtr<Scene> Scene::Create()
{
    return Memory::MakeShared<Scene>();
}

void Scene::ResetCamera(bool resetDepthRange)
{
    Vector3 center = sceneBB.GetCenter();
    camera->SetPosition(center);
    camera->SetTarget(center + Vector3(0.0f, 0.0f, -1.0f));
    camera->SetUp(Vector3::Y);

    if (resetDepthRange)
    {
        float radius = sceneBB.GetRadius();
        float nearZ = Math::Max(0.1f, radius / 750.0f);
        float farZ = radius * 50.0f;
        camera->SetNearZ(nearZ);
        camera->SetFarZ(farZ);
    }
}

void Scene::AddViewpoint()
{
    AddViewpoint(camera->GetPosition(), camera->GetTarget(), camera->GetUp());
}

void Scene::AddViewpoint(const Vector3 &position, const Vector3 &target, const Vector3 &up)
{
    viewpoints.Add({ position, target, up });
    SelectViewpoint(viewpoints.Count() - 1);
}

void Scene::RemoveViewpoint()
{
    if (currentViewpointIndex == 0)
    {
        CT_LOG(Warning, CT_TEXT("Cannot remove default viewpoint."));
        return;
    }
    viewpoints.RemoveAt(currentViewpointIndex);
    SelectViewpoint(0);
}

void Scene::SelectViewpoint(int32 index)
{
    if (index < 0 || index >= viewpoints.Count())
    {
        CT_LOG(Warning, CT_TEXT("Try to select invalid viewpoint, ignored."));
        return;
    }

    camera->SetPosition(viewpoints[index].position);
    camera->SetTarget(viewpoints[index].target);
    camera->SetUp(viewpoints[index].up);
    currentViewpointIndex = index;
}

ProgramDefines Scene::GetSceneDefines() const
{
    ProgramDefines defines;
    defines.Put(CT_TEXT("MATERIAL_COUNT"), StringConvert::ToString(materials.Count()));
    return defines;
}

SceneUpdateFlags Scene::Update(RenderContext *ctx, float currentTime)
{
    updateFlags = SceneUpdate::None;
    if (animationController->Animate(ctx, currentTime))
    {
        updateFlags |= SceneUpdate::SceneGraphChanged;

        auto &changes = animationController->GetMatricesChanged();
        for (const auto &e : meshInstanceDatas)
        {
            if (changes[e.globalMatrixID])
            {
                updateFlags |= SceneUpdate::MeshesMoved;
                break;
            }
        }
    }

    if (UpdateCamera())
        updateFlags |= SceneUpdate::CameraChanged;
    if (UpdateLights())
        updateFlags |= SceneUpdate::LightChanged;
    if (UpdateMaterials())
        updateFlags |= SceneUpdate::MaterialChanged;

    ctx->Flush();

    //TODO

    return updateFlags;
}

void Scene::Render(RenderContext *ctx, GraphicsState *state, GraphicsVars *vars, SceneRenderFlags flags)
{
    state->SetVertexArray(vao);
    vars->SetParameterBlock(sceneBlock);

    bool overrideRS = (flags & SceneRender::CustomRasterizationState) == 0;
    auto currentRS = state->GetRasterizationState();

    if (counterClockwiseDrawArgs.count)
    {
        if (overrideRS)
            state->SetRasterizationState(frontCounterClockwiseRS);
        ctx->DrawIndexedIndirect(state, vars, counterClockwiseDrawArgs.count, counterClockwiseDrawArgs.buffer.get(), 0, nullptr, 0);
    }
    if (clockwiseDrawArgs.count)
    {
        if (overrideRS)
            state->SetRasterizationState(frontClockwiseRS);
        ctx->DrawIndexedIndirect(state, vars, clockwiseDrawArgs.count, clockwiseDrawArgs.buffer.get(), 0, nullptr, 0);
    }

    if (overrideRS)
        state->SetRasterizationState(currentRS);
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
    ProgramDefines defines = GetSceneDefines();
    ProgramCompileOptions options;
    options.generateDebugInfo = true;
    auto program = Program::Create(CT_TEXT("Assets/Shaders/Scene/SceneBlock.glsl"), defines, options);
    auto reflection = program->GetReflection();
    sceneBlock = ParameterBlock::Create(reflection->GetDefaultBlockReflection());

    meshesBuffer = Buffer::CreateStructured(sizeof(MeshDesc), meshDesces.Count());
    meshInstancesBuffer = Buffer::CreateStructured(sizeof(MeshInstanceData), meshInstanceDatas.Count());
    materialsBuffer = Buffer::CreateStructured(sizeof(MaterialData), materials.Count());

    if (!lights.IsEmpty())
    {
        lightsBuffer = Buffer::CreateStructured(sizeof(LightData), lights.Count());
    }
}

void Scene::UploadResources()
{
    meshesBuffer->SetBlob(meshDesces.GetData(), 0, meshDesces.Count() * sizeof(MeshDesc));
    meshInstancesBuffer->SetBlob(meshInstanceDatas.GetData(), 0, meshInstanceDatas.Count() * sizeof(MeshInstanceData));

    sceneBlock->SetBuffer(MESH_BUFFER_NAME, meshesBuffer);
    sceneBlock->SetBuffer(MESH_INSTANCE_BUFFER_NAME, meshInstancesBuffer);

    sceneBlock->SetBuffer(STATIC_VERTEX_BUFFER_NAME, vao->GetVertexBuffer(STATIC_VERTEX_BUFFER_INDEX));
    sceneBlock->SetBuffer(PREV_VERTEX_BUFFER_NAME, vao->GetVertexBuffer(PREV_VERTEX_BUFFER_INDEX));
    sceneBlock->SetBuffer(INDEX_BUFFER_NAME, vao->GetIndexBuffer());

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

void Scene::UpdateMeshInstanceFlags()
{
    for (auto &e : meshInstanceDatas)
    {
        e.flags = CT_MESH_INSTANCE_NONE;
        const auto &m = animationController->GetGlobalMatrices()[e.globalMatrixID];
        if (DoesTransformFlip(m))
            e.flags |= CT_MESH_INSTANCE_FLIPPED;
    }
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

bool Scene::UpdateCamera(bool force)
{
    bool changed = force;

    if (cameraController)
        cameraController->Update();
    if (camera->Update())
        changed = true;

    if (changed)
    {
        sceneBlock->GetRootVar()[CAMERA_BUFFER_NAME].SetBlob(camera->GetData());
    }

    return changed;
}

bool Scene::UpdateLights(bool force)
{
    bool changed = force;

    if (changed)
    {
        Array<LightData> lightDatas;
        for (int32 i = 0; i < lights.Count(); ++i)
        {
            lightDatas.Add(lights[i]->GetData());
        }
        if (!lightDatas.IsEmpty())
        {
            lightsBuffer->SetBlob(lightDatas.GetData(), 0, sizeof(LightData) * lightDatas.Count());
        }
    }

    return changed;
}

bool Scene::UpdateMaterials(bool force)
{
    bool changed = force;

    for (int32 i = 0; i < materials.Count(); ++i)
    {
        bool updated = changed; //TODO Check material updated separately (use update frame id ?)
        if (updated)
        {
            UploadMaterial(i);
        }
    }

    return changed;
}

void Scene::CreateDrawList()
{
    Array<DrawIndexedIndirectArgs> cwArgs, ccwArgs;

    auto buffer = sceneBlock->GetBuffer(CT_TEXT("WorldMatrixBuffer"));
    const Matrix4 *matrices = (Matrix4 *)buffer->Map(BufferMapType::Read);
    for (const auto &e : meshInstanceDatas)
    {
        const auto &mesh = meshDesces[e.meshID];
        const auto &transform = matrices[e.globalMatrixID];

        DrawIndexedIndirectArgs args = {
            .indexCount = (uint32)mesh.indexCount,
            .instanceCount = 1,
            .firstIndex = (uint32)mesh.indexOffset,
            .vertexOffset = mesh.vertexOffset,
            .firstInstance = (uint32)(cwArgs.Count() + ccwArgs.Count()),
        };
        DoesTransformFlip(transform) ? cwArgs.Add(args) : ccwArgs.Add(args);
    }
    buffer->Unmap();

    if (auto count = cwArgs.Count())
    {
        clockwiseDrawArgs.buffer = Buffer::CreateIndirect(cwArgs);
        clockwiseDrawArgs.count = count;
    }
    if (auto count = ccwArgs.Count())
    {
        counterClockwiseDrawArgs.buffer = Buffer::CreateIndirect(ccwArgs);
        counterClockwiseDrawArgs.count = count;
    }
}

void Scene::Finalize()
{
    SortMeshes();
    InitResources();

    // This will bind matrix buffers.
    animationController->Animate(RenderAPI::GetDevice()->GetRenderContext(), 0);

    UpdateMeshInstanceFlags();
    UpdateBounds();
    CreateDrawList();

    UpdateCamera(true);
    AddViewpoint();
    UpdateLights(true);
    UpdateMaterials(true);
    UploadResources();

    UpdateGeometryStats();

    if (animationController->GetAnimationCount(0))
        animationController->SetActiveAnimation(0, 0);

    RasterizationStateDesc desc;
    desc.frontCCW = false;
    frontClockwiseRS = RasterizationState::Create(desc);
    desc.frontCCW = true;
    frontCounterClockwiseRS = RasterizationState::Create(desc);
}

void Scene::SortMeshes()
{
    //TODO
}

void Scene::UpdateGeometryStats()
{
    //TODO
}
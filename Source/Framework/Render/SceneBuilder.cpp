#include "Render/SceneBuilder.h"

int32 SceneBuilder::AddNode(SceneNode node)
{
    int32 nodeID = nodes.Count();
    if (node.parent != -1)
        nodes[node.parent].children.Add(nodeID);
    nodes.Add(std::move(node));
    dirty = true;
    return nodeID;
}

int32 SceneBuilder::AddMaterial(const SPtr<Material> &material)
{
    for (int32 i = 0; i < materials.Count(); ++i)
    {
        if (materials[i] == material)
            return i;
    }

    materials.Add(material);
    dirty = true;
    return materials.Count() - 1;
}

int32 SceneBuilder::AddMesh(Mesh mesh)
{
    CT_CHECK(!mesh.indices.IsEmpty());
    CT_CHECK(!mesh.positions.IsEmpty());

    meshes.Add(MeshSpec());
    auto &spec = meshes.Last();

    spec.indexOffset = buffersData.indices.Count();
    spec.staticVertexOffset = buffersData.staticDatas.Count();
    spec.dynamicVertexOffset = buffersData.dynamicDatas.Count();
    spec.indexCount = mesh.indices.Count();
    spec.vertexCount = mesh.positions.Count();
    spec.topology = mesh.topology;
    spec.materialID = AddMaterial(mesh.material);
    if (!mesh.boneIDs.IsEmpty() && !mesh.boneWeights.IsEmpty())
    {
        spec.hasDynamicData = true;
    }

    {
        buffersData.indices.AddUninitialized(spec.indexCount);
        buffersData.staticDatas.AddUninitialized(spec.vertexCount);
        buffersData.dynamicDatas.AddUninitialized(spec.hasDynamicData ? spec.vertexCount : 0);

        for (auto e : mesh.indices)
            buffersData.indices.Add(e);

        for (int32 i = 0; i < mesh.positions.Count(); ++i)
        {
            StaticVertexData s;
            s.position = mesh.positions[i];
            s.normal = mesh.normals[i];
            s.uv = mesh.uvs[i];
            s.bitangent = mesh.bitangents[i];

            buffersData.staticDatas.Add(std::move(s));

            if (spec.hasDynamicData)
            {
                DynamicVertexData d;
                d.boneID = mesh.boneIDs[i];
                d.boneWeight = mesh.boneWeights[i];
                d.staticIndex = buffersData.staticDatas.Count() - 1;
                buffersData.dynamicDatas.Add(std::move(d));
            }
        }
    }

    dirty = true;
    return meshes.Count() - 1;
}

void SceneBuilder::AddMeshInstance(int32 nodeID, int32 meshID)
{
    nodes[nodeID].meshes.Add(meshID);
    meshes[meshID].instances.Add(nodeID);
    dirty = true;
}

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

int32 SceneBuilder::AddAnimation(int32 meshID, const SPtr<Animation> &anim)
{
    meshes[meshID].animations.Add(anim);
    dirty = true;
    return meshes[meshID].animations.Count() - 1;
}

SPtr<VertexArray> SceneBuilder::CreateVao(int32 drawCount)
{
    const auto vertexCount = buffersData.staticDatas.Count();
    const auto indexCount = buffersData.indices.Count();
    uint64 ibSize = sizeof(uint32) * indexCount;
    uint64 staticVbSize = sizeof(StaticVertexData) * vertexCount;
    CT_CHECK(ibSize <= UINT32_MAX);
    CT_CHECK(staticVbSize <= UINT32_MAX);

    ResourceBindFlags ibBindFlags = ResourceBind::Index | ResourceBind::ShaderResource;
    auto ibo = Buffer::Create((uint32)ibSize, ibBindFlags, CpuAccess::None, buffersData.indices.GetData());

    ResourceBindFlags vbBindFlags = ResourceBind::Vertex | ResourceBind::UnorderedAccess | ResourceBind::ShaderResource;
    auto staticVbo = Buffer::CreateStructured(sizeof(StaticVertexData), vertexCount, vbBindFlags, CpuAccess::None, nullptr, false);
    auto prevVbo = Buffer::CreateStructured(sizeof(PrevVertexData), vertexCount, vbBindFlags, CpuAccess::None, nullptr, false);

    Array<int32> instanceIDs;
    instanceIDs.SetCount(drawCount);
    for (int32 i = 0; i < drawCount; ++i)
    {
        instanceIDs[i] = i;
    }
    auto instVbo = Buffer::Create(drawCount * sizeof(int32), ResourceBind::Vertex, CpuAccess::None, instanceIDs.GetData());

    auto staticLayout = VertexBufferLayout::Create(
        { { CT_TEXT("pos"), ResourceFormat::RGB32Float },
          { CT_TEXT("normal"), ResourceFormat::RGB32Float },
          { CT_TEXT("bitangent"), ResourceFormat::RGB32Float },
          { CT_TEXT("uv"), ResourceFormat::RG32Float } });
    auto instLayout = VertexBufferLayout::Create(
        { { CT_TEXT("meshInstanceID"), ResourceFormat::R32Int } }, true);
    auto prevLayout = VertexBufferLayout::Create(
        { { CT_TEXT("prevPos"), ResourceFormat::RGB32Float } });

    auto vertexLayout = VertexLayout::Create();
    vertexLayout->AddBufferLayout(staticLayout);
    vertexLayout->AddBufferLayout(instLayout);
    vertexLayout->AddBufferLayout(prevLayout);

    auto vao = VertexArray::Create();
    vao->SetVertexLayout(vertexLayout);
    vao->SetIndexBuffer(ibo);
    vao->SetTopology(meshes[0].topology);
    vao->AddVertexBuffer(staticVbo);
    vao->AddVertexBuffer(instVbo);
    vao->AddVertexBuffer(prevVbo);

    return vao;
}

void SceneBuilder::CreateSceneGraph(Scene *scene)
{
    scene->nodes = nodes; // Move it?
}

int32 SceneBuilder::CreateMeshData(Scene *scene)
{
    int32 drawCount = 0;

    scene->meshDesces.AddUninitialized(meshes.Count());
    scene->meshHasDynamicDatas.AddUninitialized(meshes.Count());
    for (int32 i = 0; i < meshes.Count(); ++i)
    {
        scene->meshDesces.Add(MeshDesc());
        auto &desc = scene->meshDesces.Last();

        const auto &mesh = meshes[i];
        desc.materialID = mesh.materialID;
        desc.vertexOffset = mesh.staticVertexOffset;
        desc.indexOffset = mesh.indexOffset;
        desc.vertexCount = mesh.vertexCount;
        desc.indexCount = mesh.indexCount;

        drawCount += mesh.instances.Count();

        for (const auto &instance : mesh.instances)
        {
            scene->meshInstanceDatas.Add(MeshInstanceData());
            auto &d = scene->meshInstanceDatas.Last();
            d.globalMatrixID = instance;
            d.materialID = mesh.materialID;
            d.meshID = i;
            d.vertexOffset = mesh.staticVertexOffset;
            d.indexOffset = mesh.indexOffset;
        }

        scene->meshHasDynamicDatas.Add(mesh.hasDynamicData);
        if (mesh.hasDynamicData)
        {
            CT_CHECK(mesh.instances.Count() == 1);
            for (int32 j = 0; j < mesh.vertexCount; ++j)
            {
                buffersData.dynamicDatas[mesh.dynamicVertexOffset + j].globalMatrixID = mesh.instances[0];
            }
        }
    }

    return drawCount;
}

void SceneBuilder::CreateMeshBoundingBoxes(Scene *scene)
{
    scene->meshBBs.AddUninitialized(meshes.Count());
    for (int32 i = 0; i < meshes.Count(); ++i)
    {
        const auto &mesh = meshes[i];
        int32 vertexOffset = mesh.staticVertexOffset;
        int32 vertexCount = mesh.vertexCount;

        Vector3 min(buffersData.staticDatas[vertexOffset].position);
        Vector3 max = min;
        for (int32 v = 1; v < vertexCount; ++v)
        {
            min = Math::Min(min, buffersData.staticDatas[vertexOffset + v].position);
            max = Math::Max(max, buffersData.staticDatas[vertexOffset + v].position);
        }
        scene->meshBBs.Add(AABox(min, max));     
    }
}

void SceneBuilder::CreateAnimationController(Scene *scene)
{
    //TODO
}

SPtr<Scene> SceneBuilder::GetScene()
{
    if (!dirty)
        return scene;

    CT_CHECK(!meshes.IsEmpty());

    scene = Scene::Create();

    scene->camera = camera ? camera : Camera::Create();
    scene->lights = lights;
    scene->materials = materials;
    // TODO lightProbe, envMap

    CreateSceneGraph(scene.get());
    int32 drawCount = CreateMeshData(scene.get());
    scene->vao = CreateVao(drawCount);
    CreateMeshBoundingBoxes(scene.get());
    CreateAnimationController(scene.get());
    scene->Finalize();
    dirty = false;

    return scene;
}

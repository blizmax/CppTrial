#include "Render/SceneBuilder.h"

int32 SceneBuilder::AddNode(Node node)
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
    spec.staticVertexOffset = buffersData.staticData.Count();
    spec.dynamicVertexOffset = buffersData.dynamicData.Count();
    spec.indexCount = mesh.indices.Count();
    spec.vertexCount = mesh.positions.Count();
    spec.topology = mesh.topology;
    spec.materialID = AddMaterial(mesh.material);
    if (!mesh.boneIDs.IsEmpty() && !mesh.boneWeights.IsEmpty())
    {
        spec.hasDynamicData = true;
    }

    {
        buffersData.indices.AppendUninitialized(spec.indexCount);
        buffersData.staticData.AppendUninitialized(spec.vertexCount);
        buffersData.dynamicData.AppendUninitialized(spec.hasDynamicData ? spec.vertexCount : 0);

        for (auto e : mesh.indices)
            buffersData.indices.Add(e);

        for (int32 i = 0; i < mesh.positions.Count(); ++i)
        {
            StaticVertexData s;
            s.position = mesh.positions[i];
            s.normal = mesh.normals[i];
            s.uv = mesh.uvs[i];
            s.bitangent = mesh.bitangents[i];

            buffersData.staticData.Add(std::move(s));

            if (spec.hasDynamicData)
            {
                DynamicVertexData d;
                d.boneID = mesh.boneIDs[i];
                d.boneWeight = mesh.boneWeights[i];
                d.staticIndex = buffersData.staticData.Count() - 1;
                buffersData.dynamicData.Add(std::move(d));
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

SPtr<Scene> SceneBuilder::GetScene()
{
    if (!dirty)
        return scene;

    CT_CHECK(!meshes.IsEmpty());

    scene = Scene::Create();

    //TODO

    dirty = false;
    return scene;
}

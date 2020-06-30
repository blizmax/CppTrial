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

int32 SceneBuilder::AddMesh(Mesh mesh)
{
    //TODO
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

SPtr<Scene> GetScene()
{
    //TODO
    return nullptr;
}

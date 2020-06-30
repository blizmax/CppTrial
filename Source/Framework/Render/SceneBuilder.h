#pragma once

#include "Render/Scene.h"

class SceneBuilder
{
public:
    struct Node
    {
        String name;
        Matrix4 transform;
        Matrix4 localToBindPose;
        int32 parent = - 1;
        Array<int32> children;
        Array<int32> meshes;
    };

    struct Mesh
    {
        String name;
        Array<uint32> indices;
        Array<Vector3> positions;
        Array<Vector3> normals;
        Array<Vector3> bitangents;
        Array<Vector2> uvs;
        Array<Vector4I> boneIDs;
        Array<Vector4> boneWeights;
        Topology topology = Topology::Undefined;
        SPtr<Material> material;
    };

    struct MeshSpec
    {
        Array<int32> instances;
    };

    struct BuffersData
    {
        Array<uint32> indices;
        Array<StaticVertexData> staticData;
    };

    int32 AddNode(Node node);
    int32 AddMesh(Mesh mesh);
    void AddMeshInstance(int32 nodeID, int32 meshID);
    void SetCamera(const SPtr<Camera> &newCamera);
    int32 AddLight(const SPtr<Light> &light);

    SPtr<Scene> GetScene();

private:
    SPtr<Camera> camera;
    Array<Node> nodes;
    Array<MeshSpec> meshes;
    Array<SPtr<Light>> lights;

    bool dirty = true;
};
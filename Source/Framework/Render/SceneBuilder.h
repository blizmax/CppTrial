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
        Topology topology = Topology::Undefined;
        int32 materialID = 0;
        int32 indexOffset = 0;
        int32 staticVertexOffset = 0;
        int32 dynamicVertexOffset = 0;
        int32 indexCount = 0;
        int32 vertexCount = 0;
        bool hasDynamicData = false;
        Array<int32> instances;
        Array<SPtr<Animation>> animations;
    };

    struct BuffersData
    {
        Array<uint32> indices;
        Array<StaticVertexData> staticData;
        Array<DynamicVertexData> dynamicData;
    };

public:
    int32 AddNode(Node node);
    int32 AddMesh(Mesh mesh);
    void AddMeshInstance(int32 nodeID, int32 meshID);
    void SetCamera(const SPtr<Camera> &newCamera);
    int32 AddLight(const SPtr<Light> &light);
    int32 AddAnimation(int32 meshID, const SPtr<Animation> &anim);

    SPtr<Scene> GetScene();

private:
    int32 AddMaterial(const SPtr<Material> &material);

private:
    Array<Node> nodes;
    Array<MeshSpec> meshes;
    Array<SPtr<Material>> materials;
    Array<SPtr<Light>> lights;
    SPtr<Camera> camera;

    SPtr<Scene> scene;

    BuffersData buffersData{};

    bool dirty = true;
};
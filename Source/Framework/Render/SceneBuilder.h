#pragma once

#include "Render/Scene.h"

class SceneBuilder
{
public:
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
        Array<StaticVertexData> staticDatas;
        Array<DynamicVertexData> dynamicDatas;
    };

public:
    int32 AddNode(SceneNode node);
    int32 AddMesh(Mesh mesh);
    void AddMeshInstance(int32 nodeID, int32 meshID);
    void SetCamera(const SPtr<Camera> &newCamera);
    int32 AddLight(const SPtr<Light> &light);
    int32 AddAnimation(int32 meshID, const SPtr<Animation> &anim);

    SPtr<Scene> GetScene();

private:
    int32 AddMaterial(const SPtr<Material> &material);
    SPtr<VertexArray> CreateVao(int32 drawCount);

private:
    Array<SceneNode> nodes;
    Array<MeshSpec> meshes;
    Array<SPtr<Material>> materials;
    Array<SPtr<Light>> lights;
    SPtr<Camera> camera;

    SPtr<Scene> scene;

    BuffersData buffersData{};

    bool dirty = true;
};
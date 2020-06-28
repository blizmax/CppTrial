#pragma once

#include "Render/Scene.h"

class SceneBuilder
{
public:
    struct Node
    {
        String name;
        Matrix4 transform;
        int32 parent = - 1;
        Array<int32> children;
    };

    struct Mesh
    {
        String name;
        Array<uint32> indices;
        Array<Vector3> positions;
        Array<Vector3> normals;
        Array<Vector3> bitangents;
        Array<Vector2> uvs;
        Topology topology = Topology::Undefined;
        SPtr<Material> material;
    };

    int32 AddLight(const SPtr<Light> &light);

    SPtr<Scene> GetScene();

private:
    

};
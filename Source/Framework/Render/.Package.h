#pragma once

#include "RenderCore/.Package.h"
#include "Math/Matrix4.h"
#include "Math/Quat.h"

#include "Scene/SceneTypes.h"

class Material;

struct SceneNode
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
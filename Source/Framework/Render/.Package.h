#pragma once

#include "RenderCore/.Package.h"
#include "Math/Matrix4.h"
#include "Math/Quat.h"

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

struct MeshDesc
{
    int32 vertexOffset;
    int32 indexOffset;
    int32 vertexCount;
    int32 indexCount;
    int32 materialID;
};

struct MeshInstanceData
{
    int32 globalMatrixID;
    int32 materialID;
    int32 meshID;
    int32 flags;
    int32 vertexOffset;
    int32 indexOffset;
};

struct StaticVertexData
{
    Vector3 position;
    Vector3 normal;
    Vector3 bitangent;
    Vector2 uv;
};

struct DynamicVertexData
{
    Vector4I boneID;
    Vector4 boneWeight;
    int32 staticIndex;
    int32 globalMatrixID;
};
#pragma once

#include "Math/Matrix4.h"
#include "Math/Quat.h"
#include "RenderCore/.Package.h"
#include "Scene/Shared.glsl"

class Material;

enum class LightType
{
    Point = CT_LIGHT_TYPE_POINT,
    Directional = CT_LIGHT_TYPE_DIRECTIONAL,
};

CT_DECL_FLAGS(SceneUpdate){
    None = 0,
    MeshesMoved = 1 << 0,
    SceneGraphChanged = 1 << 1,
    CameraChanged = 1 << 2,
    LightChanged = 1 << 3,
    MaterialChanged = 1 << 4,

    All = (uint32)-1,
};

struct SceneNode
{
    String name;
    Matrix4 transform;
    Matrix4 localToBindPose;
    int32 parent = -1;
    Array<int32> children;
    Array<int32> meshes;
};

struct Viewpoint
{
    Vector3 position;
    Vector3 target;
    Vector3 up;
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
#pragma once

#include "RenderCore/.Package.h"

struct MeshDesc
{
    int32 vertexOffset;
    int32 indexOffset;
    int32 vertexCount;
    int32 indexCount;
    int32 materialID;
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
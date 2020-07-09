#pragma once

#ifndef __cplusplus

#include "Utility.glsl"

#define int32 int
#define Vector2I ivec2
#define Vector3I ivec3
#define Vector4I ivec4
#define Vector2I ivec2
#define uint32 uint
#define Vector2U uvec2
#define Vector3U uvec3
#define Vector4U uvec4
#define Vector2 vec2
#define Vector3 vec3
#define Vector4 vec4
#define Matrix4 mat4
#define Color vec4
#endif

#define CT_LIGHT_TYPE_POINT       0
#define CT_LIGHT_TYPE_DIRECTIONAL 1

struct MeshInstanceData
{
    int32 globalMatrixID;
    int32 materialID;
    int32 meshID;
    int32 flags;
    int32 vertexOffset;
    int32 indexOffset;
};

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

struct MaterialData
{
    Color base;     //RGB: Base color,     A: Transparency
    Color specular; //RGB: Specular,       A: Gloss
    Color emissive; //RGB: Emissive color, A: Factor

    float alphaThreshold;
    float ior;
    float _pad[2];

#ifdef __cplusplus
    MaterialData() : 
        base{1.0f, 1.0f, 1.0f, 1.0f},
        specular{0.0f, 0.0f, 0.0f, 0.0f},
        emissive{0.0f, 0.0f, 0.0f, 1.0f},
        alphaThreshold(0.5f),
        ior(1.5f)
    {
    }
#endif
};

struct LightData
{
    Vector3 posW; // world space position
    int32 type;
    Vector3 dirW; // world space direction
    float openingAngle;
    Vector3 intensity;
    float cosOpeningAngle;
    float penumbraAngle;
    float _pad[3];

#ifdef __cplusplus
    LightData() : 
        posW{0.0f, 0.0f, 0.0f},
        type((int32)LightType::Point),
        dirW{0.0f, -1.0f, 0.0f},
        openingAngle(Math::PI),
        intensity{1.0f, 1.0f, 1.0f},
        cosOpeningAngle(-1.0f),
        penumbraAngle(0.0f)
    {
    }
#endif
};
#ifndef __CT_SCENE_PACKAGE__
#define __CT_SCENE_PACKAGE__

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

#define CT_INLINE

#include "Utility.glsl"
#include "Scene/Shared.glsl"

struct VertexData
{
    Vector3 posW;
    Vector3 normalW;
    Vector3 bitangentW;
    Vector2 uv;
    Vector3 faceNormalW;
};

struct ShadingData
{
    Vector3 posW;
    Vector3 V;
    Vector3 N;
    Vector3 T;
    Vector3 B;
    Vector2 uv;
    float NdotV;

    vec3 faceN;
    bool frontFacing;
    bool doubleSided;

    int32 materialID;
    Vector3 diffuse;
    float opacity;
    Vector3 specular;
    float linearRoughness;
    float ggxAlpha;
    Vector3 emissive;
    Vector4 occlusion;
    float IoR;
    float metallic;
    float specularTransmission;
    float eta;
};

#endif
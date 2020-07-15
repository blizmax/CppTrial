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
    vec3 posW;
    vec3 normalW;
    vec3 bitangentW;
    vec2 uv;
    vec3 faceNormalW;
};

struct ShadingData
{
    vec3 posW;
    vec3 V;
    vec3 N;
    vec3 T;
    vec3 B;
    vec2 uv;
    float NdotV;

    vec3 faceN;
    bool frontFacing;
    bool doubleSided;

    int32 materialID;
    vec3 diffuse;
    float opacity;
    vec3 specular;
    float linearRoughness;
    float ggxAlpha;
    vec3 emissive;
    vec4 occlusion;
    float IoR;
    float metallic;
    float specularTransmission;
    float eta;
};

float GetMetallic(vec3 diffuse, vec3 spec)
{
    float d = Luminance(diffuse);
    float s = Luminance(spec);
    if (s == 0) return 0;
    float a = 0.04;
    float b = s + d - 0.08;
    float c = 0.04 - s;
    float root = sqrt(b * b - 0.16 * c);
    float m = (root - b) * 12.5;
    return m;
}

#endif
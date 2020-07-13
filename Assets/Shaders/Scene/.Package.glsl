#ifndef __CT_SCENE_PACKAGE__
#define __CT_SCENE_PACKAGE__

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

struct VertexData
{
    Vector3 posW;
    Vector3 normalW;
    Vector3 bitangentW;
    Vector2 uv;
    Vector3 faceNormalW;
};

struct MaterialData
{
    Color base;     //RGB: Base color,     A: Transparency
    Color specular; //RGB: Specular,       A: Gloss
    Color emissive; //RGB: Emissive color, A: Factor

    float alphaThreshold;
    float ior;
    int32 flags;
    float _pad[1];

#ifdef __cplusplus
    MaterialData();
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
    LightData();
#endif
};

struct CameraData
{
    Matrix4 viewMat;
    Matrix4 projMat;
    Matrix4 viewProjMat;
    Matrix4 invViewProj;

    Vector3 posW; // world space position
    float focalLength;
    Vector3 up;
    float aspectRatio; // default is 1.7777(16:9)
    Vector3 target;
    float nearZ;
    Vector3 cameraU;
    float farZ;
    Vector3 cameraV;
    float jitterX;
    Vector3 cameraW;
    float jitterY;

    float frameHeight ;
    float focalDistance;
    float apertureRadius;
    float shutterSpeed;

#ifdef __cplusplus
    CameraData();
#endif
};

#ifndef __cplusplus
struct ShadingData
{
    Vector3 posW;
    Vector3 V;
    Vector3 N;
    Vector3 T;
    Vector3 B;
    Vector2 uv;
    float NdotV;

    int32 materialID;
    Vector3 diffuse;
    float opacity;
    Vector3 specular;
    float linearRoughness;
    float ggxAlpha;
    Vector3 emissive;
    Vector4 occlusion;
};
#endif

#endif
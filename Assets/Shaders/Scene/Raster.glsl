#include "Scene/Scene.glsl"
#include "Scene/ShadingData.glsl"

#define CT_VERTEX_OUT            \
    VertexOut                    \
    {                            \
        vec3 posW;               \
        vec3 normalW;            \
        vec3 bitangentW;         \
        vec2 uv;                 \
        flat int meshInstanceID; \
        flat int materialID;     \
    }


//========================== VS =============================

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inBitangent;
layout(location = 3) in vec2 inUV;
layout(location = 4) in int inMeshInstanceID;
layout(location = 5) in vec3 inPrevPos;

layout(location = 0) out CT_VERTEX_OUT vOut;

void main()
{
    mat4 worldMat = GetWorldMatrix(inMeshInstanceID);
    vec4 posW = worldMat * vec4(inPos, 1.0);
    vOut.posW = posW.xyz;

    gl_Position = GetViewProjection(camera) * posW;
    gl_Position.y = -gl_Position.y;
    gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;

    vOut.meshInstanceID = inMeshInstanceID;
    vOut.materialID = inMaterialID;
    vOut.uv = inUV;

    mat3 normalMatrix = GetInverseTransposeWorldMatrix(inMeshInstanceID);
    vOut.normalW = normalMatrix * inNormal;
    vOut.bitangentW = mat3(worldMat) * inBitangent;
}


//========================== PS =============================

// layout(location = 0) in CT_VERTEX_OUT vOut;

// VertexData PrepareVertexData(vec3 faceNormalW)
// {
//     VertexData v;
//     v.posW = vOut.posW;
//     v.normalW = vOut.normalW;
//     v.bitangentW = vOut.bitangentW;
//     v.uv = vOut.uv;
//     v.faceNormalW = faceNormalW;
//     return v;
// }

// // triangleIndex : usually use gl_PrimitiveID
// ShadingData PrepareShadingData(int triangleIndex, vec3 viewDir)
// {
//     vec3 faceNormal = GetFaceNormalW(vOut.meshInstaceID, gl_PrimitiveID);
//     VertexData v = PrepareVertexData(faceNormal);
//     return PrepareShadingData(v, vOut.materialID, viewDir);
// }

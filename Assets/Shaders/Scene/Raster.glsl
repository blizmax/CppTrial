#include "Scene/Scene.glsl"
#include "Scene/ShadingData.glsl"

#define CT_VERTEX_IN        \
    VertexIn                \
    {                       \
        vec3 pos;           \
        vec3 normal;        \
        vec3 bitangent;     \
        vec2 uv;            \
        int meshInstanceID; \
    }

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

// layout(location = 0) in CT_VERTEX_IN vIn;
// layout(location = 0) out CT_VERTEX_OUT vOut;

// void main()
// {
//     mat4 worldMat = GetWorldMatrix(vIn.meshInstanceID);
//     vec4 posW = worldMat * vec4(vIn.pos, 1.0);
//     vOut.posW = posW.xyz;

//     gl_Position = GetViewProjection(camera) * posW;
//     gl_Position.y = -gl_Position.y;
//     gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;

//     vOut.meshInstanceID = vIn.meshInstanceID;
//     vOut.materialID = vIn.materialID;
//     vOut.uv = vIn.uv;

//     mat3 normalMatrix = GetInverseTransposeWorldMatrix(vIn.meshInstanceID);
//     vOut.normalW = normalMatrix * vIn.normal;
//     vOut.bitangentW = mat3(worldMat) * vIn.bitangent;
// }


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

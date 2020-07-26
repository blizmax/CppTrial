#include "Scene/Raster.glsl"

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
    vOut.materialID = GetMaterialID(inMeshInstanceID);
    vOut.uv = inUV;

    mat3 normalMatrix = GetInverseTransposeWorldMatrix(inMeshInstanceID);
    vOut.normalW = normalMatrix * inNormal;
    vOut.bitangentW = mat3(worldMat) * inBitangent;
}
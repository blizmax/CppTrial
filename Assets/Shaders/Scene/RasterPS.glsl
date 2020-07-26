#include "Scene/Raster.glsl"
#include "Scene/ShadingData.glsl"

layout(location = 0) in CT_VERTEX_OUT vOut;

VertexData PrepareVertexData(vec3 faceNormalW)
{
    VertexData v;
    v.posW = vOut.posW;
    v.normalW = vOut.normalW;
    v.bitangentW = vOut.bitangentW;
    v.uv = vOut.uv;
    v.faceNormalW = faceNormalW;
    return v;
}

ShadingData PrepareShadingData(int triangleIndex /**gl_PrimitiveID*/, vec3 viewDir)
{
    vec3 faceNormal = GetFaceNormalW(vOut.meshInstaceID, gl_PrimitiveID);
    VertexData v = PrepareVertexData(faceNormal);
    return PrepareShadingData(v, vOut.materialID, viewDir);
}
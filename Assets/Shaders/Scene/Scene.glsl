#ifndef __CT_SCENE_SCENE__
#define __CT_SCENE_SCENE__

#include "Scene/.Package.glsl"
#include "Scene/Camera.glsl"

#ifndef MATERIAL_COUNT
#error MATERIAL_COUNT not defined!
#endif

layout(binding = 0) buffer MeshInstanceBuffer
{
    MeshInstanceData meshInstances[]; 
};

layout(binding = 1) buffer MeshDescBuffer
{
    MeshDesc meshes[];
};

layout(binding = 2) buffer WorldMatrixBuffer
{
    mat4 worldMatrices[];
};

layout(binding = 3) buffer InverseTransposeWorldMatrixBuffer
{
    mat4 inverseTransposeWorldMatrices[];
};

layout(binding = 4) buffer StaticVertexBuffer
{
    StaticVertexData vertices[];
};

layout(binding = 5) buffer IndexBuffer
{
    int indices[];
};

// Material
layout(set = 1, binding = 0) buffer MaterialBuffer
{
    MaterialData materials[];
};

layout(set = 1, binding = 1) uniform texture2D baseTextures[MATERIAL_COUNT];
layout(set = 1, binding = 2) uniform texture2D specularTextures[MATERIAL_COUNT];
layout(set = 1, binding = 3) uniform texture2D emissiveTextures[MATERIAL_COUNT];
layout(set = 1, binding = 4) uniform texture2D normalTextures[MATERIAL_COUNT];
layout(set = 1, binding = 5) uniform texture2D occlusionTexture[MATERIAL_COUNT];
layout(set = 1, binding = 6) uniform sampler samplerStates[MATERIAL_COUNT];

// Light
layout(set = 2, binding = 0) buffer LightBuffer
{
    LightData lights[];
};

// Camera
layout(set = 2, binding = 1) uniform CameraBuffer
{
    CameraData camera;
};

mat4 GetWorldMatrix(int meshInstanceID)
{
    int matrixID = meshInstances[meshInstanceID].globalMatrixID;
    return worldMatrices[matrixID];
}

mat3 GetInverseTransposeWorldMatrix(int meshInstanceID)
{
    int matrixID = meshInstances[meshInstanceID].globalMatrixID;
    return mat3(inverseTransposeWorldMatrices[matrixID]);
};

int GetMaterialCount()
{
    return MATERIAL_COUNT;
}

int GetMaterialID(int meshInstanceID)
{
    return meshInstances[meshInstanceID].materialID;
}

MaterialData GetMaterial(int materialID)
{
    return materials[materialID];
}

MeshInstanceData GetMeshInstance(int meshInstanceID)
{
    return meshInstances[meshInstanceID];
}

MeshDesc GetMeshDesc(int meshInstanceID)
{
    return meshes[meshInstances[meshInstanceID].meshID];
}

int GetLightCount()
{
    return lights.length();
}

LightData GetLight(int lightID)
{
    return lights[lightID];
}

StaticVertexData GetVertex(int index)
{
    return vertices[index];
}

ivec3 GetIndices(int meshInstanceID, int triangleIndex)
{
    ivec3 vtxIndices;
    int baseIndex = meshInstances[meshInstanceID].indexOffset + triangleIndex * 3;
    vtxIndices.x = indices[baseIndex];
    vtxIndices.y = indices[baseIndex + 1];
    vtxIndices.z = indices[baseIndex + 2];
    vtxIndices += meshInstances[meshInstanceID].vertexOffset;
    return vtxIndices;
}

vec3 GetFaceNormalW(int meshInstanceID, int triangleIndex)
{
    ivec3 vtxIndices = GetIndices(meshInstanceID, triangleIndex);
    vec3 p0 = vertices[vtxIndices[0]].position;
    vec3 p1 = vertices[vtxIndices[1]].position;
    vec3 p2 = vertices[vtxIndices[2]].position;
    vec3 N = cross(p1 - p0, p2 - p0);
    mat3 worldInvTransposeMat = GetInverseTransposeWorldMatrix(meshInstanceID);
    return normalize(worldInvTransposeMat * N);
}

#endif
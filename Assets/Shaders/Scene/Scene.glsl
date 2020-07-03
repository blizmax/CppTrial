#include "Scene/SceneTypes.h"

#ifndef MATERIAL_COUNT
#error "MATERIAL_COUNT not defined!"
#endif

layout(binding = 0) buffer MeshInstanceBuffer
{
    MeshInstanceData meshInstances[]; 
};

layout(binding = 1) buffer MeshDescBuffer
{
    MeshDesc meshes[];
};

layout(binding = 2) buffer StaticVertexBuffer
{
    StaticVertexData vertices[];
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
#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

#include "Scene/Scene.glsl"

#type pixel

layout(location = 0) out vec4 outColor;

void ActiveAllVariables()
{
    int l = meshInstances.length();
    //l = meshes.length();
    l = worldMatrices.length();
    l = inverseTransposeWorldMatrices.length();
    l = vertices.length();
    l = indices.length();
    l = prevVertices.length();

    l = materials.length();
    vec4 v = texture(sampler2D(baseTextures[0], samplerStates[0]), vec2(0.0));
    v = texture(sampler2D(specularTextures[0], samplerStates[0]), vec2(0.0));
    v = texture(sampler2D(emissiveTextures[0], samplerStates[0]), vec2(0.0));
    v = texture(sampler2D(normalTextures[0], samplerStates[0]), vec2(0.0));
    v = texture(sampler2D(occlusionTextures[0], samplerStates[0]), vec2(0.0));

    l = lights.length();
    CameraData cd = camera;
}

void main()
{
    ActiveAllVariables();

    outColor = vec4(0.0);
}
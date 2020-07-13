#ifndef __CT_SCENE_SHADING_DATA__
#define __CT_SCENE_SHADING_DATA__

#include "Scene/.Package.glsl"
#include "Scene/Scene.glsl"
#include "Scene/TextureSampler.glsl"

bool EvalBasicAlphaTest(float alpha, float threshold)
{
    return alpha < threshold;
}

void ApplyAlphaTest(int flags, float alpha, float threshold, vec3 posW)
{
    if (EvalBasicAlphaTest(alpha, threshold))
        discard;
}

ShadingData PrepareShadingData(VertexData v, int materialID, vec3 viewDir)
{
    ShadingData sd;

    vec4 baseColor = SampleTexture(baseTextures[materialID], samplerStates[materialID], v.uv);
    sd.opacity = baseColor.a;
    ApplyAlphaTest(materials[materialID].flags, baseColor.a, materials[materialID].alphaThreshold, v.posW);

    //TODO
}

#endif
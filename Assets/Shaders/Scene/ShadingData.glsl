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
    MaterialData md = GetMaterial(materialID);

    vec4 baseColor = SampleTexture(baseTextures[materialID], samplerStates[materialID], v.uv, md.base, GetMaterialSampleMode(md.flags, CT_MAT_DIFFUSE));
    sd.opacity = baseColor.a;
    ApplyAlphaTest(materials[materialID].flags, baseColor.a, materials[materialID].alphaThreshold, v.posW);

    sd.posW = v.posW;
    sd.uv = v.uv;
    sd.V = viewDir;
    sd.N = normalize(v.normalW);

    sd.faceN = v.faceNormalW;
    sd.frontFacing = dot(sd.V, sd.faceN) >= 0.0;
    sd.doubleSided = (md.flags & CT_MAT_DOUBLE_SIDED) > 0;

    bool validTangentSpace = dot(v.bitangentW, v.bitangentW) > 0.0;
    if (validTangentSpace)
    {
        sd.B = normalize(v.bitangent - sd.N * (dot(v.bitangentW, sd.N)));
        sd.T = normalize(sd.B, sd.N);
    }

    sd.materialID = materialID;
    sd.IoR = md.IoR;
    sd.specularTransmission = md.specularTransmission;
    sd.eta = sd.frontFacing ? (1.0 / sd.IoR) : sd.IoR;

    sd.occlusion = 1.0f;
    bool sampleOcclusion = GetMaterialSampleMode(md.flags, CT_MAT_OCCLUSION) > 0;
    vec4 spec = SampleTexture(specularTexture[materialID], samplerStates[materialID], v.uv, md.specular, GetMaterialSampleMode(md.flags, CT_MAT_SPECULAR));
    if (GetMaterialShadingModel(md.flags) == CT_SHADING_MODEL_METAL_ROUGH)
    {
        sd.diffuse = baseColor.rgb;

        // default IoR = 1.5, F0 = 0.04
        float f = (sd.IoR - 1.f) / (sd.IoR + 1.f);
        float F0 = f * f;
  
        sd.metallic = spec.b;
        sd.specular = mix(F0, baseColor.rgb, sd.metallic);
        sd.linearRoughness = spec.g;
        if (sampleOcclusion)
            sd.occlusion = spec.r;
    }
    else // CT_SHADING_MODEL_SPEC_GLOSS
    {
        sd.diffuse = baseColor.rgb;
        sd.specular = spec.rgb;
        sd.linearRoughness = 1.0 - spec.b;
        sd.metallic = GetMetallic(); //TODO
        if (sampleOcclusion)
            sd.occluson = SampleTexture(occlusionTexture[materialID], samplerStates[materialID], v.uv, vec4(1.0), CT_MAT_SAMPLE_MODE_TEXTURE).r;
    }

    //TODO
}

#endif
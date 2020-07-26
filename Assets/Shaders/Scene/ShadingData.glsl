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

vec3 RGB2Normal(vec3 rgb)
{
    vec3 n = rgb * 2.0 - 1.0;
    return normalize(n);
}

vec3 RG2Normal(vec2 rg)
{
    // use r^2 + g^2 + z^2 = 1.0
    vec3 n;
    n.xy = rg * 2.0 - 1.0;
    // saturate because error from BC5 can break the sqrt
    n.z = saturate(dot(rg, rg));
    n.z = sqrt(1.0 - n.z);
    return normalize(n);
}

void ApplyNormalMap(MaterialData md, inout ShadingData sd)
{
    int mode = GetMaterialNormalMode(md.flags);
    if (mode == CT_MAT_NORMAL_MODE_NONE)
        return;

    vec3 mapN = SampleTexture(normalTextures[materialID], samplerStates[materialID], sd.uv).rgb;
    if (mode == CT_MAT_NORMAL_MODE_RGB)
        mapN = RGB2Normal(mapN);
    else // CT_MAT_NORMAL_MODE_RG
        mapN = RG2Normal(mapN);

    sd.N = sd.T * mapN.x + sd.B * mapN.y + sd.N * mapN.z; // == mat3(T, B, N) * mapN
    sd.B = normalize(sd.B - sd.N * dot(sd.B, sd.N));
    sd.T = normalize(cross(sd.B, sd.N));
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
        sd.B = normalize(v.bitangentW - sd.N * (dot(v.bitangentW, sd.N)));
        sd.T = normalize(cross(sd.B, sd.N));
    }

    sd.materialID = materialID;
    sd.IoR = md.IoR;
    sd.specularTransmission = md.specularTransmission;
    sd.eta = sd.frontFacing ? (1.0 / sd.IoR) : sd.IoR;

    sd.occlusion = vec4(1.0);
    bool sampleOcclusion = GetMaterialSampleMode(md.flags, CT_MAT_OCCLUSION) > 0;
    vec4 spec = SampleTexture(specularTextures[materialID], samplerStates[materialID], v.uv, md.specular, GetMaterialSampleMode(md.flags, CT_MAT_SPECULAR));
    if (GetMaterialShadingModel(md.flags) == CT_SHADING_MODEL_METAL_ROUGH)
    {
        sd.diffuse = baseColor.rgb;

        // default IoR = 1.5, F0 = 0.04
        float f = (sd.IoR - 1.0) / (sd.IoR + 1.0);
        vec3 F0 = vec3(f * f);
  
        sd.metallic = spec.b;
        sd.specular = mix(F0, baseColor.rgb, sd.metallic);
        sd.linearRoughness = spec.g;
        if (sampleOcclusion)
            sd.occlusion = vec4(spec.r);
    }
    else // CT_SHADING_MODEL_SPEC_GLOSS
    {
        sd.diffuse = baseColor.rgb;
        sd.specular = spec.rgb;
        sd.linearRoughness = 1.0 - spec.b;
        sd.metallic = GetMetallic(sd.diffuse, sd.specular);
        if (sampleOcclusion)
            sd.occlusion = SampleTexture(occlusionTextures[materialID], samplerStates[materialID], v.uv, vec4(1.0), CT_MAT_SAMPLE_MODE_TEXTURE);
    }

    sd.linearRoughness = max(0.08, sd.linearRoughness);
    sd.ggxAlpha = sd.linearRoughness * sd.linearRoughness;

    if (sd.frontFacing)
    {
        vec4 emissive = SampleTexture(emissiveTextures[materialID], samplerStates[materialID], v.uv, vec4(md.emissive.rgb, 1.0), GetMaterialSampleMode(md.flags, CT_MAT_EMISSIVE));
        sd.emissive = emissive.rgb * md.emissive.a;
    }

    if (validTangentSpace)
    {
        ApplyNormalMap(md, sd);
    }

    sd.NdotV = dot(sd.N, sd.V);
    if (!sd.frontFacing && sd.doubleSided)
    {
        sd.N = -sd.N;
        sd.NdotV = -sd.NdotV;
    }

    return sd;
}

#endif
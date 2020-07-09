#pragma once

#include "Scene/.Package.glsl"
#include "Scene/ShadingData.glsl"
#include "Scene/Light.glsl"
#include "Scene/BRDF.glsl"

struct ShadingResult
{
    vec3 diffuseBrdf;
    vec3 specularBrdf;
    vec3 diffuse;
    vec3 specular;
    vec4 color;
};

ShadingResult InitShadingResult()
{
    ShadingResult sr;
    sr.diffuse = 0.0;
    sr.color.rgb = 0.0;
    sr.color.a = 1.0;
    sr.specular = 0.0;
    sr.diffuseBrdf = 0.0;
    sr.specularBrdf = 0.0;
    return sr;
}

ShadingResult EvalMaterial(ShadingData sd, LightData light, float shadowFactor)
{
    ShadingResult sr = InitShadingResult();
    LightSample ls = EvalLight(light, sd);

    if (ls.NdotL <= 0.0)
        return sr;
    sd.NdotV = saturate(sd.NdotV);

    //TODO


    return sr;
}
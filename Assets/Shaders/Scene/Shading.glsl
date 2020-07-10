#ifndef __CT_SCENE_SHADING__
#define __CT_SCENE_SHADING__

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

    sr.diffuseBrdf = EvalDiffuseBrdf(sd, ls);
    sr.diffuse = ls.diffuse * sr.diffuseBrdf * ls.NdotL;
    sr.color.rgb = sr.diffuse;
    sr.color.a = sd.opacity;

    sr.specularBrdf = EvalSpecularBrdf(sd, ls);
    sr.specular = ls.specular * sr.specularBrdf * ls.NdotL;
    sr.color.rgb += sr.specular;
    
    sr.color.rgb *= sr.shadowFactor;

    return sr;
}


#endif
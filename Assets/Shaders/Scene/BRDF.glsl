#ifndef __CT_SCENE_BRDF__
#define __CT_SCENE_BRDF__

#include "Scene/ShadingData.glsl"
#include "Scene/Light.glsl"

vec3 FresnelSchlick(vec3 f0, vec3 f90, flot u)
{
    return f0 + (f90 - f0) * pow(1.0 - u, 5.0);
}

vec3 EvalDiffuseLambertBrdf(ShadingData sd, LightSample ls)
{
    return sd.diffuse.rgb / PI;
}

vec3 EvalDiffuseBrdf(ShadingData sd, LightSample ls)
{
    return EvalDiffuseLambertBrdf(sd, ls);
}

vec3 EvalGGX(float ggxAlpha, float NdotH)
{
    float a2 = ggxAlpha * ggxAlpha;
    float d = (NdotH * a2 - NdotH) * NdotH + 1.0;
    return a2 / (d * d);
}

vec3 EvalSmithGGX(float ggxAlpha, float NdotL, float NdotV)
{
    float a2 = ggxAlpha * ggxAlpha;
    float ggxv = NdotL * sqrt((-NdotV * a2 + NdotV) * NdotV + a2);
    float ggxl = NdotV * sqrt((-NdotL * a2 + NdotL) * NdotL + a2);
    return 0.5 / (ggxv + ggxl);
}

vec3 EvalSpecularBrdf(ShadingData sd, LightSample ls)
{
    float ggxAlpha = sd.ggxAlpha;

    float D = EvalGGX(ggxAlpha, ls.NdotH);
    float G = EvalSmithGGX(ggxAlpha, ls.NdotL, sd.NdotV);
    vec3 F = FresnelSchlick(sd.specular, 1.0, saturate(ls.LdotH));
    return D * G * F / PI;
}

#endif
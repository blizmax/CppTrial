#ifndef __CT_SCENE_BRDF__
#define __CT_SCENE_BRDF__

#include "Scene/Light.glsl"

vec3 FresnelSchlick(vec3 f0, vec3 f90, float u)
{
    return f0 + (f90 - f0) * pow(1.0 - u, 5.0);
}

float DistributionGGX(float ggxAlpha, float NdotH)
{
    float a2 = ggxAlpha * ggxAlpha;
    float d = (NdotH * a2 - NdotH) * NdotH + 1.0;
    return a2 / max(0.001, PI * d * d);
}

float GeometrySchlickGGX(float d, float ggxAlpha)
{
    float r = (ggxAlpha + 1.0);
    float k = (r * r) / 8.0;
    return d / (d * (1.0 - k) + k);
}

float GeometrySmith(float ggxAlpha, float NdotL, float NdotV)
{
    float ggx1 = GeometrySchlickGGX(NdotL, ggxAlpha);
    float ggx2 = GeometrySchlickGGX(NdotV, ggxAlpha);
    return ggx1 * ggx2;
}

#endif
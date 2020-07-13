#ifndef __CT_SCENE_LIGHT__
#define __CT_SCENE_LIGHT__

#include "Scene/.Package.glsl"

struct LightSample
{
    vec3 diffuse;
    vec3 specular;
    vec3 L;
    vec3 posW;
    float NdotH;
    float NdotL;
    float LdotH;
    float distance;
};

void CalcCommonLightProperties(ShadingData sd, inout LightSample ls)
{
    vec3 H = normalize(sd.V + ls.L);
    ls.NdotH = dot(sd.N, H);
    ls.NdotL = dot(sd.N, ls.L);
    ls.LdotH = dot(ls.L, H);
}

LightSample EvalDirectionalLight(LightData light, vec3 surfacePosW)
{
    LightSample ls;
    ls.diffuse = light.intensity;
    ls.specular = light.intensity;
    ls.L = -normalize(light.dirW);
    float dist = length(surfacePosW - light.posW);
    ls.posW = surfacePosW - light.dirW * dist;
    return ls;
}

LightSample EvalPointLight(LightData light, vec3 surfacePosW)
{
    LightSample ls;
    ls.posW = light.posW;
    ls.L = light.posW - surfacePosW;

    float dist2 = dot(ls.L, ls.L);
    ls.distance = (dist2 > 0.0001) ? length(ls.L) : 0.0;
    ls.L = (dist2 > 0.0001) ? normalize(ls.L) : 0.0;

    float falloff = 1.0 / (0.0001 + dist2);
    float cosTheta = -dot(ls.L, light.dirW);
    if (cosTheta < light.cosOpeningAngle)
    {
        falloff = 0.0;
    }
    else if (light.penumbraAngle > 0.0)
    {
        float delta = light.openingAngle - acos(cosTheta);
        falloff *= saturate((delta - light.penumbraAngle) / light.penumbraAngle); //linear attenuation
    }
    ls.diffuse = light.intensity * falloff;
    ls.specular = ls.distance;
    return ls;
}

LightSample EvalLight(LightData light, ShadingData sd)
{
    LightSample ls;
    if (light.type == CT_LIGHT_TYPE_POINT)
    {
        ls = EvalPointLight(light, sd.posW);
    }
    else if (light.type == CT_LIGHT_TYPE_DIRECTIONAL)
    {
        ls = EvalDirectionalLight(light, sd.posw);
    }
    CalcCommonLightProperties(sd, ls);
    return ls;
}

#endif
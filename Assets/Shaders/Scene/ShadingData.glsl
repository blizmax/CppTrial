#ifndef __CT_SCENE_SHADING_DATA__
#define __CT_SCENE_SHADING_DATA__

struct ShadingData
{
    vec3 posW;
    vec3 V;
    vec3 N;
    vec3 T;
    vec3 B;
    vec2 uv;
    float NdotV;

    int materialID;
    vec3 diffuse;
    float opacity;
    vec3 specular;
    float linearRoughness;
    float ggxAlpha;
    vec3 emissive;
    vec4 occlusion;

};


#endif
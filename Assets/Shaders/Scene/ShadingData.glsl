#pragma once

struct ShadingData
{
    vec3 posW;
    vec3 V;
    vec3 N;
    vec3 T;
    vec3 B;
    vec2 uv;
    float NdotV;
};

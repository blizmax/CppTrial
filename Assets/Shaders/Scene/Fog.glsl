#ifndef __CT_SCENE_FOG__
#define __CT_SCENE_FOG__

#include "Scene/Camera.glsl"

float CalcFogAmount(float distToEye, float fogStart, float fogEnd)
{
    return saturate((distToEye - fogStart) / (fogEnd - fogStart));
}

#endif
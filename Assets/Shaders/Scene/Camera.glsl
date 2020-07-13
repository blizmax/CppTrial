#ifndef __CT_SCENE_CAMERA__
#define __CT_SCENE_CAMERA__

#include "Scene/.Package.glsl"

vec3 GetCameraPosition(CameraData cd)
{
    return cd.posW;
}

mat4 GetViewProjection(CameraData cd)
{
    return cd.viewProjMat;
}

#endif
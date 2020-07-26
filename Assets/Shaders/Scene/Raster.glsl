#ifndef __CT_SCENE_RASTER__
#define __CT_SCENE_RASTER__

#include "Scene/Scene.glsl"

#define CT_VERTEX_OUT            \
    VertexOut                    \
    {                            \
        vec3 posW;               \
        vec3 normalW;            \
        vec3 bitangentW;         \
        vec2 uv;                 \
        flat int meshInstanceID; \
        flat int materialID;     \
    }

#endif
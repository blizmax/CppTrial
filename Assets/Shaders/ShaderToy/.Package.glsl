#include "Utility.glsl"

layout(binding = 0) uniform GBlock
{
    mat4 Model;
    mat4 View;
    mat4 Projection;
    float Time;
};
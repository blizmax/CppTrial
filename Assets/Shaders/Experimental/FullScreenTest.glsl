#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

#include "Passes/FullScreen.glsl"

#type vertex

CT_FULL_SCREEN_VS

#type pixel

CT_FULL_SCREEN_PS

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(0.0);
}

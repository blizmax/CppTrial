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

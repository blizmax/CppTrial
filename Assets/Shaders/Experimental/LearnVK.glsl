#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

#type vertex

#include "Scene/RasterVS.glsl"

#type pixel

#include "Scene/RasterPS.glsl"

layout(location = 0) out vec4 outColor;

void main() 
{
    vec3 viewDir = normalize(GetCameraPosition(camera) - vOut.posW);
    ShadingData sd = PrepareShadingData(gl_PrimitiveID, viewDir);

    vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);
    for (int i = 0; i < GetLightCount(); ++i)
    {
        finalColor.rgb += EvalMaterial(sd, GetLight(i), 1.0).color.rgb;
    }
    
    outColor = finalColor;
}
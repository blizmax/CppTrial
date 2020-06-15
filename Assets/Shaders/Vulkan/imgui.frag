#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 UV;
layout(location = 1) in vec4 Color;

layout(location = 0) out vec4 FragmentColor;

layout(binding = 1) uniform sampler mainSampler;
layout(binding = 2) uniform texture2D mainTex;

void main()
{
    FragmentColor = Color * texture(sampler2D(mainTex, mainSampler), UV);
}
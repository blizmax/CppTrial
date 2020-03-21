#type vertex
#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexUV;

out vec2 UV;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    gl_Position = Projection * View * Model * vec4(VertexPosition, 1.0);
    UV = VertexUV; 
}

#type fragment
#version 330 core

layout(location = 0) out vec4 FragmentColor;

#define PI 3.1415927

in vec2 UV;

uniform sampler2D Texture;

uniform float Progress = 0.5;

void main()
{
    vec2 uv = UV - 0.5;
    float len = length(uv);

    float v = smoothstep(0.495, 0.5, 1 - len);

    float ang = atan(-uv.y, uv.x);
    float percent = fract(ang * 0.5 / PI + 0.25);

    v *= 1 - 0.5 * step(Progress, percent);

    FragmentColor = v * vec4(UV, 0.0, 1.0);
}
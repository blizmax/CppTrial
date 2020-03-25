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

#include "Assets/Shaders/Utility.glsl"
#include "Assets/Shaders/3D.glsl"

in vec2 UV;

uniform sampler2D Texture;
uniform float Time;

float Point(Ray r, vec3 p)
{
    float d = length(p - ClosestPoint(r, p));
    return smoothstep(0.05, 0.045, d);
}

float Cube(Ray r)
{
    float v = 0;
    v += Point(r, vec3(0.0, 0.0, 0.0));
    v += Point(r, vec3(0.0, 0.0, 1.0));
    v += Point(r, vec3(0.0, 1.0, 0.0));
    v += Point(r, vec3(0.0, 1.0, 1.0));
    v += Point(r, vec3(1.0, 0.0, 0.0));
    v += Point(r, vec3(1.0, 0.0, 1.0));
    v += Point(r, vec3(1.0, 1.0, 0.0));
    v += Point(r, vec3(1.0, 1.0, 1.0));

    return saturate(v);
}

void main()
{
    vec2 uv = UV - 0.5;
   
    vec3 camPos = vec3(3.0 * sin(Time), 2.0, 3.0 * cos(Time));
    vec3 target = vec3(0.5, 0.5, 0.5);
    float zoom = 1.0;

    Ray r = GetRay(uv, camPos, target, zoom);
    float v = Cube(r);

    FragmentColor = vec4(vec3(0.8, 0.5, 0.0) * v, 1.0);
}
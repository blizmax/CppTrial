#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

#include "ShaderToy/.Package.glsl"

#type vertex
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexUV;

layout(location = 0) out vec2 UV;

void main()
{
    gl_Position = Projection * View * Model * vec4(VertexPosition, 1.0);
    UV = VertexUV; 
}

#type fragment
layout(location = 0) out vec4 FragmentColor;

layout(location = 0) in vec2 UV;

layout(binding = 1) uniform sampler Sampler;
layout(binding = 2) uniform texture2D Texture;

float Smooth(vec2 p, vec2 p0, vec2 p1)
{
    vec2 v0 = normalize(p1 - p0);
    vec2 v1 = p - p0;
    vec2 proj = dot(v0, v1) * v0;
    vec2 prep = v1 - proj;
    float dist = length(prep);

    return smoothstep(0.02, 0, dist);
}

// float Smooth(vec2 p, vec2 p0, vec2 p1)
// {
//     float dist = length(p - p0);
//     return smoothstep(0.01, 0, dist);
// }

void Pow(inout vec2 p0, inout vec2 p1)
{
    p0.y = pow(p0.x, 3);
    p1.y = pow(p1.x, 3);
}

void Sqrt(inout vec2 p0, inout vec2 p1)
{
    p0.y = sqrt(p0.x);
    p1.y = sqrt(p1.x);
}

void Log(inout vec2 p0, inout vec2 p1)
{
    p0.y = log(p0.x);
    p1.y = log(p1.x);
}

void Sin(inout vec2 p0, inout vec2 p1)
{
    p0.y = sin(p0.x);
    p1.y = sin(p1.x);
}

void Tan(inout vec2 p0, inout vec2 p1)
{
    p0.y = tan(p0.x);
    p1.y = tan(p1.x);
}

void Ceil(inout vec2 p0, inout vec2 p1)
{
    p0.y = ceil(p0.x);
    p1.y = ceil(p1.x);
}

void Floor(inout vec2 p0, inout vec2 p1)
{
    p0.y = floor(p0.x);
    p1.y = floor(p1.x);
}

void Round(inout vec2 p0, inout vec2 p1)
{
    p0.y = round(p0.x);
    p1.y = round(p1.x);
}

void Trunc(inout vec2 p0, inout vec2 p1)
{
    p0.y = trunc(p0.x);
    p1.y = trunc(p1.x);
}

void Fract(inout vec2 p0, inout vec2 p1)
{
    p0.y = fract(p0.x);
    p1.y = fract(p1.x);
}

void Mod(inout vec2 p0, inout vec2 p1)
{
    p0.y = mod(p0.x, 1.5);
    p1.y = mod(p1.x, 1.5);
}

void Abs(inout vec2 p0, inout vec2 p1)
{
    p0.y = abs(p0.x);
    p1.y = abs(p1.x);
}

void Sign(inout vec2 p0, inout vec2 p1)
{
    p0.y = sign(p0.x);
    p1.y = sign(p1.x);
}

void Smoothstep(inout vec2 p0, inout vec2 p1)
{
    p0.y = 3 * smoothstep(-1.5, 2.5, p0.x);
    p1.y = 3 * smoothstep(-1.5, 2.5, p1.x);
}

void Clamp(inout vec2 p0, inout vec2 p1)
{
    p0.y = clamp(p0.x, -2, 3);
    p1.y = clamp(p1.x, -2, 3);
}

void Step(inout vec2 p0, inout vec2 p1)
{
    p0.y = step(-2.7, p0.x);
    p1.y = step(-2.7, p1.x);
}

// Lerp
void Mix(inout vec2 p0, inout vec2 p1)
{
    p0.y = mix(1, 0.9, p0.x);
    p1.y = mix(1, 0.9, p1.x);
}

float Axis(vec2 p)
{
    return smoothstep(0.02, 0, abs(p.x)) + smoothstep(0.02, 0, abs(p.y));
}

void main()
{
    vec4 dummy = texture(sampler2D(Texture, Sampler), UV);

    vec2 uv = (UV - 0.5) * 9.7;
    vec2 p0 = uv;
    vec2 p1 = uv + 0.0001;

    Abs(p0, p1);
    float v = Smooth(uv, p0, p1);

    float axis = Axis(uv);
    FragmentColor = vec4(axis, axis, axis, 1.0);
    FragmentColor.g += v;
}
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

#include "Assets/Shaders/Utility.glsl"
#include "Assets/Shaders/Random.glsl"

layout(location = 0) out vec4 FragmentColor;

in vec2 UV;

uniform float Time;
uniform sampler2D Texture;
uniform vec4 SkyColor;
uniform vec4 MoonColor;
uniform vec4 CloudColor;

float Circle(vec2 uv, vec2 pos, float radius, float blur)
{
    float dist = length(uv - pos);
    return smoothstep(radius, radius - blur, dist);
}

vec4 Moon(vec2 uv)
{
    float t = Circle(uv, vec2(0.7, 0.78), 0.1, 0.01);
    return MoonColor * t;
}

vec4 Sky(vec2 uv)
{
    vec4 col = SkyColor;

    uv += 0.1;
    uv *= 10.0;
    vec2 gridUV = fract(uv) - 0.5;
    vec2 gridID = floor(uv);

    float fade = remap(-1.0, 1.0, 0.1, 0.6, sin(Time * 2.0 + TWO_PI * random21(gridID)));
    float mask = step(3.0, uv.y) * step((1.28 - uv.y / 10.0), random22(gridID).y);
    vec2 p = random22(gridID) - 0.5;
    float d = length(gridUV - p);
    vec4 starColor = vec4(fade) * mask;
    col += starColor * smoothstep(0.06, 0.04, d);

    return col;
}

float Cloud(vec2 uv, vec2 pos, float radius)
{
    pos.y += 0.05 * radius * sin(Time * 0.66 + TWO_PI * random11(radius));
    return Circle(uv, pos, radius, 0.01);
}

vec4 Clouds(vec2 uv)
{
    float t = Cloud(uv, vec2(0.58, 1.45), 0.70);
    t += Cloud(uv, vec2(0.15, 1.08), 0.48);
    t += Cloud(uv, vec2(0.91, 1.35), 0.66);
    return vec4(CloudColor.xyz, CloudColor.a * t);
}

void main()
{
    vec2 uv = UV;
    vec4 sky = Sky(uv);
    vec4 moon = Moon(uv);
    vec4 clouds = Clouds(uv);

    vec4 col = mix(sky, moon, moon.a);
    col = mix(col, clouds, clouds.a);

    FragmentColor = vec4(col.rgb, 1.0);
}
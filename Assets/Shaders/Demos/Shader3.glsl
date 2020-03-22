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

in vec2 UV;

uniform sampler2D Texture;

float Circle(vec2 uv, vec2 pos, float radius, float blur)
{
    float dist = length(uv - pos);
    return smoothstep(radius, radius - blur, dist);
}

float Band(float t, float a, float b, float blur)
{
    float v0 = smoothstep(a - blur, a, t);
    float v1 = smoothstep(b, b - blur, t);
    return v0 * v1;
}

float Rectangle(vec2 uv, vec2 pos, vec2 size, float blur)
{
    return Band(uv.x, pos.x, pos.x + size.x, blur) *
        Band(uv.y, pos.y, pos.y + size.y, blur);
}

float Tick(vec2 uv, vec2 pos, float blur)
{
    uv.x *= 0.75;
    float c0 = Circle(uv - vec2(0, uv.x * 0.6), pos, 0.2, blur);
    float c1 = Circle(uv - vec2(0, uv.x * 0.6), pos + vec2(0.018, 0.08), 0.215, blur);
    return c0 - c1;
}

void main()
{
    vec2 uv = UV - 0.5;

    //float t = Rectangle(vec2(uv.x, uv.y + 0.05 * sin(20.0 * uv.x)), vec2(-0.2, -0.2), vec2(0.6, 0.6), 0.01);
    //t -= Circle(uv, vec2(-0.2, -0.2), 0.3, 0.01);
    float t = Tick(uv, vec2(0.0, 0.0), 0.01);

    vec3 color = vec3(1.0) * t;

    FragmentColor = vec4(color, 1.0);
}
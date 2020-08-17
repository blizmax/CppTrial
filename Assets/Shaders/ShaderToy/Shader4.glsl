#type vertex

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

layout(location = 0) out vec4 FragmentColor;

#include "Assets/Shaders/Utility.glsl"

in vec2 UV;

uniform sampler2D Texture;


void main()
{
    vec2 uv = UV - 0.5;
    float d = length(uv);

    float ang = atan(uv.y, uv.x);
    float h = ang / TWO_PI + step(0.0, -ang);
    float s = min(0.5, d) * 2.0;
    float v = 1.0;
    vec3 rgb = hsv2rgb(vec3(h, s, v));

    float outer = smoothstep(0.5, 0.495, d);
    float inner = smoothstep(0.40, 0.395, d);
    float ring = outer - inner;

    FragmentColor = vec4(ring * rgb, 1.0);
}
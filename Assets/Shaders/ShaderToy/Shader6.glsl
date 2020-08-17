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
#include "Assets/Shaders/Random.glsl"

in vec2 UV;

uniform sampler2D Texture;
uniform float Time;

void main()
{
    vec2 uv = UV * 10.0;
    uv += Time * 0.5;

    float c = 0.0;
    for(float i = 0.0; i < 5.0; ++i)
    {
        float n = vnoise(uv * pow(2, i));
        c += n * pow(0.5, i);
    }
    c /= 2.0;

    vec3 col = vec3(c);
    FragmentColor = vec4(col, 1.0);
}
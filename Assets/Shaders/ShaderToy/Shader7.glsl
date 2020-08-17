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
    float t = Time * 1.0;

    vec2 gridUV = fract(uv) - 0.5;
    vec2 gridIndex = floor(uv);

    float minDist = 9999.0;

    for(float y = -1.0; y <= 1.0; ++y)
    {
        for(float x= -1.0; x <= 1.0; ++x)
        {
            vec2 offset = vec2(x, y);
            vec2 n = random22(gridIndex + offset);
            vec2 p = offset + sin(t * n) * 0.5;
            float d = length(gridUV - p);
            if(d < minDist)
            {
                minDist = d;
            }
        }
    }

    vec3 col = vec3(minDist);

    FragmentColor = vec4(col, 1.0);
}
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

layout(binding = 3) uniform UBlock
{
    float Progress;
};

void main()
{
    vec4 dummy = texture(sampler2D(Texture, Sampler), UV);

    vec2 uv = UV - 0.5;
    float len = length(uv);

    float v = smoothstep(0.495, 0.5, 1 - len);

    float ang = atan(-uv.y, uv.x);
    float percent = fract(ang * 0.5 / PI + 0.25);

    v *= 1 - 0.5 * step(Progress, percent);

    FragmentColor = v * vec4(UV, 0.0, 1.0);
}
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler Sampler[2];
layout(binding = 1) uniform texture2D MainTex[2];

// layout(binding = 2) buffer Block
// {
//     mat4 Mat;
//     vec4 Col;
//     float Scl;
// };

layout(binding = 2) uniform Block
{
    mat4 Mat;
    vec4 Col;
    //float Scl;
};

layout(binding = 3) uniform Block2
{
    float Scl;
} NamedBlock;

layout(location = 0) in vec2 UV;

layout(location = 0) out vec4 FragColor;

void main()
{
    vec4 v = Mat * Col;
    v *= NamedBlock.Scl;

    vec4 col = texture(sampler2D(MainTex[1], Sampler[1]), UV);
    FragColor = texture(sampler2D(MainTex[0], Sampler[0]), UV) + col * v;
}

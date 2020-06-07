#version 450
#extension GL_ARB_separate_shader_objects : enable

struct Textures
{
    texture2D t[2];
};

layout(binding = 0) uniform sampler samplers[2];
layout(binding = 1) uniform texture2D textures[2];

layout(binding = 2) uniform Block
{
    mat4 mat;
    vec4 vec;
};

struct Inner
{
    float scls[2];
};

struct Misc
{
    vec4 write;
    Inner inner;
    float s;
};

layout(binding = 3, set = 1) uniform Block2
{
    Misc misc;
} namedBlock;

layout(location = 0) in vec2 UV;

layout(location = 0) out vec4 FragColor;

void main()
{
    vec4 v = mat * vec;
    v *= namedBlock.misc.inner.scls[0];
    v *= namedBlock.misc.inner.scls[1];
    v *= namedBlock.misc.s;

    //namedBlock.misc.write = v;

    vec4 col = texture(sampler2D(textures[1], samplers[1]), UV);
    FragColor = texture(sampler2D(textures[0], samplers[0]), UV) + col * v;
}

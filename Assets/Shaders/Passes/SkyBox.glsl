layout(binding = 0) uniform PerFrameCB
{
    mat4 world;
    mat4 view;
    mat4 proj;
    float scale;
};

layout(binding = 1) uniform textureCube gTexture;
layout(binding = 2) uniform sampler gSampler;

#type vertex

layout(location = 0) in vec3 inPos;
layout(location = 0) out vec3 dir;

void main()
{
    dir = inPos;
    vec4 pos = proj * view * world * vec4(inPos, 1.0);
    pos.xy *= scale;

    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
}

#type pixel

layout(location = 0) in vec3 dir;
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = textureLod(samplerCube(gTexture, gSampler), normalize(dir), 0.0);
}
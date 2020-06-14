#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UB
{
    float v;
};

layout(location = 0) in vec3 Color;

layout(location = 0) out vec4 FragmentColor;

void main() {
    vec3 col = vec3(v);

    FragmentColor = vec4(col, 1.0);
}
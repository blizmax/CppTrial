#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

layout(binding = 0) uniform UB
{
    mat4 mvp;
};

#type vertex

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;

layout(location = 0) out vec3 Normal;

void main() 
{
    gl_Position = mvp * vec4(VertexPosition, 1.0);
    gl_Position.y = -gl_Position.y;
    gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;

    Normal = VertexNormal;
}


#type pixel

layout(location = 0) in vec3 Normal;

layout(location = 0) out vec4 FragmentColor;

void main() 
{
    vec4 col = vec4(0.0, 1.0, 0.0, 1.0);
    FragmentColor = col;
}
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexUV;

layout(location = 0) out vec2 UV;

// layout(set = 0, binding = 0) uniform UBlock
// {
//     mat4 MVP;
// };

void main() 
{
    gl_Position = vec4(VertexPosition, 1.0);
    UV = VertexUV;
}
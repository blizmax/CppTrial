#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 VertexPosition;
layout(location = 1) in vec3 VertexColor;

layout(location = 0) out vec3 Color;

// layout(set = 0, binding = 0) uniform UBlock
// {
//     mat4 MVP;
// };

void main() {
    gl_Position = vec4(VertexPosition, 0.0, 1.0);
    Color = VertexColor;
}
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 VertexPosition;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec4 VertexColor;

layout(location = 0) out vec2 UV;
layout(location = 1) out vec4 Color;

layout(binding = 0) uniform UB
{
    mat4 mvp;
};

void main()
{
    gl_Position = mvp * vec4(VertexPosition, 0.0, 1.0);
	UV = VertexUV;
    Color = VertexColor; 
}
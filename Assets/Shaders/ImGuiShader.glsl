#type vertex
#version 330 core

layout(location = 0) in vec2 VertexPosition;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec4 VertexColor;

out vec2 UV;
out vec4 Color;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(VertexPosition, 0.0, 1.0);
	UV = VertexUV;
    Color = VertexColor; 
}

#type fragment
#version 330 core

layout(location = 0) out vec4 FragmentColor;

in vec2 UV;
in vec4 Color;

uniform sampler2D MainTex;

void main()
{
	FragmentColor = Color * texture(MainTex, UV);
}
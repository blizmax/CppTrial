#type vertex
#version 330 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inColor;

out vec2 uv;
out vec4 color;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(inPosition, 0.0, 1.0);
	uv = inUV;
    color = inColor; 
}

#type fragment
#version 330 core

layout(location = 0) out vec4 outColor;

in vec2 uv;
in vec4 color;

uniform sampler2D mainTex;

void main()
{
	outColor = color * texture(mainTex, uv);
}
#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 uv;

void main()
{
	gl_Position = vec4(position, 1.0);
	uv = texCoord; 
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 uv;

uniform sampler2D mainTex;

void main()
{
	//color = texture(mainTex, uv);
	color = vec4(uv, 0.0, 1.0);
}
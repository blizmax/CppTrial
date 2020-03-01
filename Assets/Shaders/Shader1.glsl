#type vertex
#version 330 core

layout(location = 0) in vec3 position;

void main()
{
	gl_Position = vec4(position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.5, 0.5, 0.0, 1.0);
}
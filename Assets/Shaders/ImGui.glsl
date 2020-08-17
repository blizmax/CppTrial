#type vertex
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
    gl_Position.y = -gl_Position.y;
    gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;

	UV = VertexUV;
    Color = VertexColor; 
}

#type pixel
layout(location = 0) in vec2 UV;
layout(location = 1) in vec4 Color;

layout(location = 0) out vec4 FragmentColor;

layout(binding = 1) uniform sampler mainSampler;
layout(binding = 2) uniform texture2D mainTex;

void main()
{
    FragmentColor = Color * texture(sampler2D(mainTex, mainSampler), UV);
}
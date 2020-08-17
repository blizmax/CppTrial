//#include "Scene/Raster.glsl"

// struct Textures
// {
//     texture2D t[2];
// };

// layout(binding = 0) uniform sampler samplers[2];
// layout(binding = 1) uniform texture2D textures[2];

// layout(binding = 2) uniform Block
// {
//     mat4 mat;
//     vec4 vec;
// };

// struct Inner
// {
//     float scls[2];
// };

// struct Misc
// {
//     vec3 write;
//     Inner inner;
//     float s;
// };

// layout(binding = 3, set = 1) uniform Block2
// {
//     Misc miscs[1][1];
// } namedBlock;

// layout (binding = 3) uniform ExampleBlock
// {
//                      // base alignment ----------    // aligned offset
//     float value;     // 4                            // 0
//     vec3 vector;     // 16                           // 16 (vec4)
//     mat4 matrix;     // 16                           // 32 (line 0)
//                      // 16                           // 48 (line 1)
//                      // 16                           // 64 (line 2)
//                      // 16                           // 80 (line 3)
//     float values[3]; // 16 (element rounded to vec4) // 96 (values[0])
//                      // 16                           // 112 (values[1])
//                      // 16                           // 128 (values[2])
//     bool boolean;    // 4                            // 144
//     int integer;     // 4                            // 148
// };

// layout (binding = 4) uniform ArrayBlock
// {
//     float arrayData[7];
// };

// #type vertex


// layout(location = 0) in vec3 inPos;
// layout(location = 1) in vec3 inNormal;
// layout(location = 2) in vec3 inBitangent;
// layout(location = 3) in vec2 inUV;
// layout(location = 4) in int inMeshInstanceID;
// layout(location = 5) in vec3 inPrevPos;

// void main()
// {
//     gl_Position = vec4(0.0);
// }

// #type pixel

// //layout(location = 0) in CT_VERTEX_OUT vOut;
// layout(location = 0) in VertexOut
// {
//     vec2 uv;
// } vOut;

// layout(location = 0) out vec4 FragColor;

// void main()
// {
//     vec4 v = mat * vec;
//     v *= namedBlock.miscs[0][0].inner.scls[0];
//     v *= namedBlock.miscs[0][0].inner.scls[1];
//     v *= namedBlock.miscs[0][0].s;
//     //namedBlock.misc.write = v;

//     v *= value;

//     v *= arrayData[1];

//     vec4 col = texture(sampler2D(textures[1], samplers[1]), vOut.uv);
//     FragColor = texture(sampler2D(textures[0], samplers[0]), vOut.uv) + col * v;
// }

#type compute

layout(local_size_x = 32, local_size_y = 32) in;

void main()
{

}
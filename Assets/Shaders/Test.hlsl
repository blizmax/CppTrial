struct VsIn
{
    [[vk::location(0)]]float2 pos : POSITION;
    [[vk::location(1)]]float2 uv : TEXCOORD0;
    [[vk::location(2)]]float4 color : COLOR;
};

struct VsOut
{
    float2 uv : TEXCOORD0;
    float4 color : COLOR;
    float4 pos : SV_POSITION;
};

[[vk::binding(0)]]
cbuffer PerFrameCB
{
    float4x4 mvp;
};

#type vertex

VsOut main(VsIn vIn)
{
    VsOut vOut;
    vOut.pos = mul(float4(vIn.pos, 0.0, 1.0), mvp);
    vOut.pos.y = -vOut.pos.y;
    vOut.pos.z = (vOut.pos.z + vOut.pos.w) / 2.0;

    vOut.uv = vIn.uv;
    vOut.color = vIn.color;

    return vOut;
}

// #type pixel

// [[vk::binding(1)]]
// Texture2D gTexture;

// [[vk::binding(2)]]
// SamplerState gSampler;

// float4 main(VsOut vOut) : SV_TARGET
// {
//     float4 outColor = vOut.color * gTexture.Sample(gSampler, vOut.uv);
//     return outColor;
// }
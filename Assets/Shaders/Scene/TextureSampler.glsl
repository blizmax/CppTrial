#ifndef __CT_TEXTURE_SAMPLER__
#define __CT_TEXTURE_SAMPLER__

vec4 SampleTexture(in texture2D t, in sampler s, vec2 uv)
{
    return texture(sampler2D(t, s), uv);
}

vec4 SampleTexture(in texture2D t, in sampler s, vec2 uv, vec4 c, int mode)
{
    if (mode == CT_MAT_SAMPLE_MODE_NONE)
        return vec4(0.0);
    if (mode == CT_MAT_SAMPLE_MODE_CONST)
        return c;
    return texture(sampler2D(t, s), uv);
}

#endif
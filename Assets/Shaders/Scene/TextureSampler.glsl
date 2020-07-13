#ifndef __CT_TEXTURE_SAMPLER__
#define __CT_TEXTURE_SAMPLER__

vec4 SampleTexture(in texture2D t, in sampler s, vec2 uv)
{
    return texture(sampler2D(t, s), uv);
}


#endif
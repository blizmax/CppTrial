float random11(float p)
{
    return fract(sin(123.4 * p) * 6789.123);
}

float random21(vec2 p)
{
    return fract(sin(dot(p, vec2(123.4, 567.8))) * 6789.123);
}

vec2 random22(vec2 p)
{
    vec2 xy = vec2(dot(p, vec2(123.4, 567.8)), dot(p, vec2(3456.7, 8901.2)));
    return fract(sin(xy) * 6789.123);
}

float vnoise(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);
    f = smoothstep(0.0, 1.0, f);

    float bl = random21(i);
    float br = random21(i + vec2(1.0, 0.0));
    float b = mix(bl, br, f.x);

    float tl = random21(i + vec2(0.0, 1.0));
    float tr = random21(i + vec2(1.0, 1.0));
    float t = mix(tl, tr, f.x);

    return mix(b, t, f.y);
}
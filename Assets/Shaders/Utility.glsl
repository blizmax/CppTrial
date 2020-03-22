#define PI 3.14159265359
#define TWO_PI 6.28318530718

#define saturate(x) clamp(x, 0.0, 1.0)

float remap01(float a, float b, float t)
{
    return saturate((t - a) / (b - a));
}

float remap(float a, float b, float c, float d, float t)
{
    return saturate((t - a) / (b - a)) * (d - c) + c;
}

struct Ray
{
    vec3 o;
    vec3 d;
};

Ray GetRay(vec2 uv, vec3 pos, vec3 target, float zoom)
{
    vec3 f = normalize(target - pos);
    vec3 r = cross(vec3(0.0, 1.0, 0.0), f);
    vec3 u = cross(f, r);
    vec3 c = pos + zoom * f;
    vec3 i = c + uv.x * r + uv.y * u;
    
    Ray ray;
    ray.o = pos;
    ray.d = normalize(i - pos);
    return ray;
}

vec3 ClosestPoint(Ray r, vec3 p)
{
    vec3 v = max(0.0, dot(p - r.o, r.d)) * r.d;
    return r.o + v;
}
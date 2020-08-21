#include "SharedDefines.glsl"

#define CT_PARTICLE_EMIT_THREADS 64
#define CT_PARTICLE_SORT_THREADS 1024

struct Particle
{
    Vector3 pos;
    float scale;
    Vector3 vel;
    float life;
    Vector3 accel;
    float growth;
    float rot;
    float rotVel;
    Vector2 padding;
};

struct EmitData
{
    int32 emitCount;
    int32 maxParticles;
    Vector2 padding;
};

struct SortData
{
    int32 index;
    float depth;
    Vector2 padding;
};
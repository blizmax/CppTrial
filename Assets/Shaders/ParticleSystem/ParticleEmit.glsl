
#type compute

layout(binding = 0) buffer DeadListBuffer
{
    uint deadList[];
};

layout(binding = 1) buffer PoolBuffer
{
    Particle particlePool[];
};

layout(binding = 2) buffer EmitBuffer
{
    Particle emitList[];
};

layout(local_size_x = CT_PARTICLE_EMIT_THREADS) in;

void main()
{

}
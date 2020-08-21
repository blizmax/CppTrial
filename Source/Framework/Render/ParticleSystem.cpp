#include "Render/ParticleSystem.h"

SPtr<ParticleSystem> ParticleSystem::Create(RenderContext *ctx, const ParticleSystemDesc &desc)
{
    return Memory::MakeShared<ParticleSystem>(ctx, desc);
}

ParticleSystem::ParticleSystem(RenderContext *ctx, const ParticleSystemDesc &desc)
    : desc(desc)
{
    //TODO
}

void ParticleSystem::Emit(RenderContext *ctx, int32 num)
{
    //TODO
}
#pragma once

#include "RenderCore/ProgramVars.h"
#include "RenderCore/RenderContext.h"
#include "ParticleSystem/Shared.glsl"

struct ParticleSystemDesc
{
    int32 maxParticles;
    int32 maxEmitPerFrame;
    bool sorted;
};

class ParticleSystem
{
public:
    struct EmitterData
    {
        float duration;
        float durationOffset;
        int32 emitCount;
        int32 emitCountOffset;
        Vector3 spawnPos;
        Vector3 spawnPosOffset;
        Vector3 vel;
        Vector3 velOffset;
        Vector3 accel;
        Vector3 accelOffset;
        float scale;
        float scaleOffset;
        float growth;
        float growthOffset;
        float billboardRotation;
        float billboardRotationOffset;
        float billboardRotationVel;
        float billboardRotationVelOffset;
    };

    struct EmitResources
    {
        SPtr<ComputeVars> vars;
        SPtr<ComputeState> state;
    };

    struct SimulateResources
    {
        SPtr<ComputeVars> vars;
        SPtr<ComputeState> state;
    };

    struct DrawResources
    {
        SPtr<GraphicsVars> vars;
        SPtr<GraphicsState> state;
        SPtr<VertexArray> vao;
    };

    ParticleSystem(RenderContext *ctx, const ParticleSystemDesc &desc);

    void Update(RenderContext *ctx, float time, const Matrix4 &view);
    void Render(RenderContext *ctx, const SPtr<FrameBuffer> &fbo, const Matrix4 &view, const Matrix4 &proj);

    static SPtr<ParticleSystem> Create(RenderContext *ctx, const ParticleSystemDesc &desc);

private:
    void Emit(RenderContext *ctx, int32 num);

private:
    ParticleSystemDesc desc;

    EmitResources emitResources;
    SimulateResources simulateResources;
    DrawResources drawResources;
};
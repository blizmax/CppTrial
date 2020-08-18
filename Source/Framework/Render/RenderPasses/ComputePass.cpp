#include "Render/RenderPasses/ComputePass.h"
#include "RenderCore/ComputeContext.h"

SPtr<ComputePass> ComputePass::Create(const ProgramDesc &desc)
{
    return Memory::MakeShared<ComputePass>(desc);
}

SPtr<ComputePass> ComputePass::Create(const String &path, const ProgramDefines &defines)
{
    auto desc = Program::CreateDesc(path, defines);
    return Create(desc);
}

ComputePass::ComputePass(const ProgramDesc &desc)
{
    auto program = Program::Create(desc);
    state = ComputeState::Create();
    state->SetProgram(program);
    vars = ComputeVars::Create(program);
}

void ComputePass::Execute(ComputeContext *ctx, int32 threadsX, int32 threadsY, int32 threadsZ)
{
    auto threadGroupSize = GetThreadGroupSize();
    int32 x = (threadsX + threadGroupSize.x - 1) / threadGroupSize.x;
    int32 y = (threadsY + threadGroupSize.y - 1) / threadGroupSize.y;
    int32 z = (threadsZ + threadGroupSize.z - 1) / threadGroupSize.z;
    ctx->Dispatch(state.get(), vars.get(), { x, y, z });
}

void ComputePass::Execute(ComputeContext *ctx, const Vector3I threads)
{
    Execute(ctx, threads.x, threads.y, threads.z);
}

void ComputePass::ExecuteIndirect(ComputeContext *ctx, const Buffer *argBuffer, uint64 argBufferOffset)
{
    ctx->DispatchIndirect(state.get(), vars.get(), argBuffer, argBufferOffset);
}

Vector3I ComputePass::GetThreadGroupSize() const
{
    return GetProgram()->GetReflection()->GetThreadGroupSize();
}
#include "Render/RenderPasses/FullScreenPass.h"
#include "RenderCore/RenderAPI.h"

SPtr<FullScreenPass> FullScreenPass::Create(const ProgramDesc &desc)
{
    return Memory::MakeShared<FullScreenPass>(desc);
}

SPtr<FullScreenPass> FullScreenPass::Create(const String &path, const ProgramDefines &defines)
{
    auto desc = Program::CreateDesc(path, defines);
    return Create(desc);
}

FullScreenPass::FullScreenPass(const ProgramDesc &desc)
    : GraphicsPass(desc)
{
    auto dsState = DepthStencilState::Create({ .depthReadEnabled = false });
    state->SetDepthStencilState(dsState);

    state->SetVertexArray(RenderAPI::GetDefaultQuadVertexArray());
}

void FullScreenPass::Execute(RenderContext *ctx, const SPtr<FrameBuffer> &fbo, bool setVpSc)
{
    state->SetFrameBuffer(fbo, setVpSc);
    ctx->Draw(state.get(), vars.get(), 4, 0);
}
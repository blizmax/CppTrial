#pragma once

#include "Render/RenderPasses/GraphicsPass.h"

class FullScreenPass : public GraphicsPass
{
public:
    FullScreenPass(const ProgramDesc &desc);

    virtual void Execute(RenderContext *ctx, const SPtr<FrameBuffer> &fbo, bool setVpSc = true);

    static SPtr<FullScreenPass> Create(const ProgramDesc &desc);
    static SPtr<FullScreenPass> Create(const String &path, const ProgramDefines &defines = {});
};
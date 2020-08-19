#pragma once

#include "Render/RenderPass.h"

class SkyBoxPass : public RenderPass
{
public:
    virtual RenderPassReflection Reflect(const RenderGraph::CompileContext &ctx) override;

    static SPtr<SkyBoxPass> Create();

private:

};
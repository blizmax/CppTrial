#pragma once

#include "Application/.Package.h"
#include "Utils/Module.h"

class RenderManager : public Module
{
public:
    RenderManager() : Module(CT_TEXT("RenderManager")) {}
    virtual ~RenderManager() = default;

    virtual void OnLoad() override {}
    virtual void OnUnload() override {}
    virtual void OnUpdate() override {}
};

extern RenderManager *gRenderManager;
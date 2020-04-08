#pragma once

#include "Application/.Package.h"
#include "Utils/Module.h"

class RenderManager : public Module
{
public:
    RenderManager() = default;
    virtual ~RenderManager() = default;

    virtual void Startup() override {}
    virtual void Shutdown() override {}
    virtual void Tick() override {}

    virtual String GetName() const override
    {
        return CT_TEXT("RenderManager");
    }
};

extern RenderManager *gRenderManager;
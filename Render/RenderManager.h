#pragma once

#include "Application/.Package.h"
#include "Utils/Module.h"

class RenderManager : public Module
{
public:
    RenderManager() = default;
    virtual ~RenderManager() = default;

    virtual void OnLoad() override {}
    virtual void OnUnload() override {}
    virtual void OnUpdate() override {}

    virtual String GetName() const override
    {
        return CT_TEXT("RenderManager");
    }
};

extern RenderManager *gRenderManager;
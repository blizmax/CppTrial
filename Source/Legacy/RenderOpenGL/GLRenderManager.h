#pragma once

#include "RenderOpenGL/.Package.h"
#include "Render/RenderManager.h"

class GLRenderManager : public RenderManager
{
public:
    virtual void Startup() override;
    virtual void Shutdown() override;
};
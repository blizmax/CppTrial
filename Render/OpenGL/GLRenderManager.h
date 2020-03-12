#pragma once

#include "Render/OpenGL/.Package.h"
#include "Render/RenderManager.h"

class GLRenderManager : public RenderManager
{
public:
    virtual void OnLoad() override;
    virtual void OnUpdate() override;
};
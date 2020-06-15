#pragma once

#include "Render/.Package.h"
#include "Application/.Package.h"
#include "Utils/Module.h"
#include "RenderCore/RenderAPI.h"

class RenderManager : public Module
{
public:
    RenderManager() = default;
    virtual ~RenderManager() = default;

    virtual void Startup() override;
    virtual void Shutdown() override;
    virtual void Tick() override;
    
    void Present();
    RenderContext *GetRenderContext() const;

    const SPtr<FrameBuffer> &GetTargetFrameBuffer() const
    {
        return targetFrameBuffer;
    }

    virtual String GetName() const override
    {
        return CT_TEXT("RenderManager");
    }

private:
    SPtr<FrameBuffer> targetFrameBuffer;
};

extern RenderManager *gRenderManager;
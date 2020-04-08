#pragma once

#include "Application/.Package.h"
#include "Utils/Module.h"

class DebugManager : public Module
{
public:
    DebugManager() = default;
    virtual ~DebugManager() = default;

    virtual void Startup() override;
    virtual void Shutdown() override;
    virtual void Tick() override;

    virtual String GetName() const override
    {
        return CT_TEXT("DebugManager");
    }
};

extern DebugManager *gDebugManager;
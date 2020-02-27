#pragma once

#include "Application/.Package.h"
#include "Utils/Module.h"

class DebugManager : public Module
{
public:
    DebugManager() = default;
    virtual ~DebugManager() = default;

    virtual void OnLoad() override;
    virtual void OnUnload() override;
    virtual void OnUpdate() override;

    virtual String GetName() const override
    {
        return CT_TEXT("DebugManager");
    }
};

extern DebugManager *gDebugManager;
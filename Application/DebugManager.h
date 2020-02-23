#pragma once

#include "Application/Module.h"

class DebugManager : public Module
{
public:
    DebugManager() : Module(CT_TEXT("DebugManager")) {}
    virtual ~DebugManager() = default;

    virtual void OnLoad() override;
    virtual void OnUnload() override;
    virtual void OnTick() override;
};

extern DebugManager *gDebugManager;
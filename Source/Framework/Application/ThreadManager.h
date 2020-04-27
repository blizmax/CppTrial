#pragma once

#include "Application/.Package.h"
#include "Core/Thread.h"
#include "Utils/Module.h"

class ThreadManager : public Module
{
public:
    virtual void Startup() override;
    virtual void Shutdown() override;
    virtual void Tick() override;

    void RunThread(const String &name, const ThreadFunc &func);

    String GetName() const override
    {
        return CT_TEXT("ThreadManager");
    }
};

extern ThreadManager *gThreadManager;
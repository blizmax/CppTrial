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

    void Execute(const String &name, ThreadPool::WorkFunc func);

    String GetName() const override
    {
        return CT_TEXT("ThreadManager");
    }

private:
    ThreadPool pool;
};

extern ThreadManager *gThreadManager;
#pragma once

#include "Utils/.Package.h"

class Module
{
public:
    Module() = default;
    virtual ~Module() = default;

    Module(Module &&) = delete;
    Module(const Module &) = delete;
    Module &operator=(Module &&) = delete;
    Module &operator=(const Module &) = delete;

    virtual void Startup() = 0;
    virtual void Shutdown() = 0;
    virtual void Tick() = 0;

    virtual String GetName() const = 0;
};
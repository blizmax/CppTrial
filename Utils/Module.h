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

    virtual void OnLoad() = 0;
    virtual void OnUnload() = 0;
    virtual void OnUpdate() = 0;

    virtual String GetName() const = 0;
};
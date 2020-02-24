#pragma once

#include "Utils/.Package.h"

class Module
{
public:
    Module(const String &name) : name(name) {}
    virtual ~Module() = default;

    Module(Module &&) = delete;
    Module(const Module &) = delete;
    Module &operator=(Module &&) = delete;
    Module &operator=(const Module &) = delete;

    virtual void OnLoad() = 0;
    virtual void OnUnload() = 0;
    virtual void OnUpdate() = 0;

    const String &GetName() const
    {
        return name;
    }

private:
    String name;
};
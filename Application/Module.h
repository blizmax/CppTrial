#pragma once

#include "Application/.Package.h"

class Module
{
public:
    Module(const String &name) : name(name)
    {
    }

    virtual ~Module() = default;

    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void OnTick() = 0;

    //TODO
    //virtual void OnEvent() = 0;

    const String &GetName() const
    {
        return name;
    }

private:
    String name;
};
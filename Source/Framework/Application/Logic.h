#pragma once

#include "Application/.Package.h"
#include "Utils/Module.h"

class Logic : public Module
{
public:
    virtual ~Logic() = default;

    virtual String GetName() const override
    {
        return CT_TEXT("Logic");
    }
};

extern Logic *gLogic;
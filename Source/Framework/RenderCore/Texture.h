#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

class Texture
{
public:
    virtual ~Texture() = default;

    static SPtr<Texture> Create();
};

}
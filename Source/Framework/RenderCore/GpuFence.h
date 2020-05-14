#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

class GpuFence
{
public:
    virtual ~GpuFence() = default;


    static SPtr<GpuFence> Create();
};


}
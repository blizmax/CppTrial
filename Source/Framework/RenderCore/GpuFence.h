#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

class GpuFence
{
public:
    virtual ~GpuFenc() = default;


    static SPtr<GpuFence> Create();
};


}
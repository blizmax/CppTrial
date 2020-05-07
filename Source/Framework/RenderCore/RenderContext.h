#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
class RenderContext
{
public:
    virtual ~RenderContext() = default;

    virtual bool ResourceBarrier(const Resource *resource);
    virtual void UpdateResource();

    virtual void Flush();

};
}
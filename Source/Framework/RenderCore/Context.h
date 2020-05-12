#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
class Context
{
public:
    virtual ~Context() = default;

    virtual void Init() = 0;
    virtual void Destroy() = 0;


    virtual bool ResourceBarrier(const Resource *resource);
    virtual void UpdateResource();

    virtual void Flush();

};
}
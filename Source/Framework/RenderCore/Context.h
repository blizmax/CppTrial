#pragma once

#include "RenderCore/Resource.h"
#include "RenderCore/RenderWindow.h"

namespace RenderCore
{

class Context
{
public:
    virtual ~Context() = default;

    virtual void Init() = 0;
    virtual void Destroy() = 0;


    // virtual bool ResourceBarrier(const Resource *resource);
    // virtual void UpdateResource();

    // virtual void Flush();

protected:
    bool BufferBarrier(const Buffer *buffer, ResourceState newState);
    bool TextureBarrier(const Texture *texture, ResourceState newState);

};
}
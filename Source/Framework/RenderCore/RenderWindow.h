#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

/** Represents the render relevant part of the window. */    
class RenderWindow
{
public:
    virtual ~RenderWindow() = default;

    virtual uint32 GetPositionX() const = 0;
    virtual uint32 GetPositionY() const = 0;
    virtual uint32 GetWidth() const = 0;
    virtual uint32 GetHeight() const = 0;
};

}
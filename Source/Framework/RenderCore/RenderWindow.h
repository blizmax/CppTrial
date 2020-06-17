#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

/** Represents the render relevant part of the window. */    
class RenderWindow
{
public:
    virtual ~RenderWindow() = default;

    virtual int32 GetPositionX() const = 0;
    virtual int32 GetPositionY() const = 0;
    virtual int32 GetWidth() const = 0;
    virtual int32 GetHeight() const = 0;
};

}
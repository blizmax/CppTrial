#pragma once

#include "Application/WindowEvent.h"
#include "Utils/Module.h"

class Logic : public Module
{
public:
    virtual ~Logic() = default;

    virtual String GetName() const override
    {
        return CT_TEXT("Logic");
    }

    virtual void OnWindowResized(WindowResizedEvent &e)
    {
    }

    virtual void OnKeyTyped(KeyTypedEvent &e)
    {
    }
    virtual void OnKeyDown(KeyDownEvent &e)
    {
    }
    virtual void OnKeyUp(KeyUpEvent &e)
    {
    }
    virtual void OnTouchDown(TouchDownEvent &e)
    {
    }
    virtual void OnTouchUp(TouchUpEvent &e)
    {
    }
    virtual void OnMouseMoved(MouseMovedEvent &e)
    {
    }
    virtual void OnMouseScrolled(MouseScrolledEvent &e)
    {
    }
};

extern Logic *gLogic;
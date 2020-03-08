#pragma once

#include "Application/InputEvent.h"

class Input
{
public:
    using InputEventHandler = Delegate<void(InputEvent &)>;

    InputEventHandler keyTypedEventHandler;
    InputEventHandler keyDownEventHandler;
    InputEventHandler keyUpEventHandler;
    InputEventHandler touchDownEventHandler;
    InputEventHandler touchUpEventHandler;
    InputEventHandler mouseMovedEventHandler;
    InputEventHandler mouseScrolledEventHandler;

    int32 GetMouseX() const
    {
        return mouseX;
    }

    int32 GetMouseY() const
    {
        return mouseY;
    }

    int32 GetDeltaX() const
    {
        return deltaX;
    }

    int32 GetDeltaY() const
    {
        return deltaY;
    }

    bool IsKeyDown(int32 key) const
    {
        if (key < 0 || key > CT_KEY_LAST)
            return false;
        return pressedKeys[key];
    }

    bool IsButtonDown(int32 button) const
    {
        if (button < 0 || button > CT_BUTTON_LAST)
            return false;
        return pressedButtons[button];
    }

protected:
    virtual void ProcessKeyTyped(int32 character);
    virtual void ProcessKeyDown(int32 key);
    virtual void ProcessKeyUp(int32 key);
    virtual void ProcessTouchDown(int32 button);
    virtual void ProcessTouchUp(int32 button);
    virtual void ProcessMouseMoved(int32 posX, int32 posY);
    virtual void ProcessMouseScrolled(int32 amount);
    // void ProcessKeyTyped(int32 character);
    // void ProcessKeyDown(int32 key);
    // void ProcessKeyUp(int32 key);
    // void ProcessTouchDown(int32 button);
    // void ProcessTouchUp(int32 button);
    // void ProcessMouseMoved(int32 posX, int32 posY);
    // void ProcessMouseScrolled(int32 amount);

protected:
    bool pressedKeys[512] = {0};
    bool pressedButtons[8] = {0};
    int32 mouseX = 0;
    int32 mouseY = 0;
    int32 deltaX = 0;
    int32 deltaY = 0;

    friend class InputManager;
};
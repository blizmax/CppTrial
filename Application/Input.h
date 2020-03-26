#pragma once

#include "Application/InputEvent.h"

class Input
{
public:
    using KeyTypedEventDelegate = Delegate<void(KeyTypedEvent &)>;
    using KeyDownEventDelegate = Delegate<void(KeyDownEvent &)>;
    using KeyUpEventDelegate = Delegate<void(KeyUpEvent &)>;
    using TouchDownEventDelegate = Delegate<void(TouchDownEvent &)>;
    using TouchUpEventDelegate = Delegate<void(TouchUpEvent &)>;
    using MouseMovedEventDelegate = Delegate<void(MouseMovedEvent &)>;
    using MouseScrolledEventDelegate = Delegate<void(MouseScrolledEvent &)>;

    KeyTypedEventDelegate keyTypedHandler;
    KeyDownEventDelegate keyDownHandler;
    KeyUpEventDelegate keyUpHandler;
    TouchDownEventDelegate touchDownHandler;
    TouchUpEventDelegate touchUpHandler;
    MouseMovedEventDelegate mouseMovedHandler;
    MouseScrolledEventDelegate mouseScrolledHandler;

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

    static int32 TranslateKey(int32 key)
    {
        if (key < 0 || key > CT_KEY_LAST)
            return CT_KEY_UNKNOWN;
        return keycodeTable[key];
    }

    static void Init();

protected:
    virtual void ProcessKeyTyped(int32 character);
    virtual void ProcessKeyDown(int32 key);
    virtual void ProcessKeyUp(int32 key);
    virtual void ProcessTouchDown(int32 posX, int32 posY, int32 button);
    virtual void ProcessTouchUp(int32 posX, int32 posY, int32 button);
    virtual void ProcessMouseMoved(int32 posX, int32 posY);
    virtual void ProcessMouseScrolled(int32 amount);

    virtual void RelasePressed();

protected:
    static int32 keycodeTable[512];

    bool pressedKeys[512] = {0};
    bool pressedButtons[8] = {0};
    int32 mouseX = 0;
    int32 mouseY = 0;
    int32 deltaX = 0;
    int32 deltaY = 0;
};
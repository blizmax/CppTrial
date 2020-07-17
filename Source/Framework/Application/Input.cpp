#include "Application/Input.h"

void Input::ProcessKeyTyped(int32 character)
{
    KeyTypedEvent event((wchar)character);
    keyTypedHandler(event);
}

void Input::ProcessKeyDown(int32 key)
{
    bool repeat = false;
    key = TranslateKey(key);

    if (key != CT_KEY_UNKNOWN)
    {
        repeat = pressedKeys[key];
        pressedKeys[key] = true;
    }

    KeyDownEvent event(key, repeat);
    keyDownHandler(event);
}

void Input::ProcessKeyUp(int32 key)
{
    key = TranslateKey(key);

    if (key != CT_KEY_UNKNOWN)
        pressedKeys[key] = false;

    KeyUpEvent event(key);
    keyUpHandler(event);
}

void Input::ProcessTouchDown(int32 posX, int32 posY, int32 button)
{
    if (button < 0 || button > CT_BUTTON_LAST)
        button = CT_BUTTON_UNKNOWN;

    if (button != CT_BUTTON_UNKNOWN)
        pressedButtons[button] = true;

    TouchDownEvent event(posX, posY, button);
    touchDownHandler(event);
}

void Input::ProcessTouchUp(int32 posX, int32 posY, int32 button)
{
    if (button < 0 || button > CT_BUTTON_LAST)
        button = CT_BUTTON_UNKNOWN;

    if (button != CT_BUTTON_UNKNOWN)
        pressedButtons[button] = false;

    TouchUpEvent event(posX, posY, button);
    touchUpHandler(event);
}

void Input::ProcessMouseMoved(int32 posX, int32 posY)
{
    deltaX = posX - mouseX;
    deltaY = posY - mouseY;
    mouseX = posX;
    mouseY = posY;

    MouseMovedEvent event(mouseX, mouseY);
    mouseMovedHandler(event);
}

void Input::ProcessMouseScrolled(int32 amount)
{
    MouseScrolledEvent event(amount);
    mouseScrolledHandler(event);
}

void Input::RelasePressed()
{
    //TODO should emit event?

    for (int32 key = 0; key <= CT_KEY_LAST; ++key)
    {
        if (pressedKeys[key])
        {
            pressedKeys[key] = false;
        }
    }

    for (int32 button = 0; button <= CT_BUTTON_LAST; ++button)
    {
        if (pressedButtons[button])
        {
            pressedButtons[button] = false;
        }
    }
}
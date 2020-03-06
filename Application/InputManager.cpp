#include "Application/InputManager.h"

void InputManager::OnLoad()
{
}

void InputManager::OnUnload()
{
}

void InputManager::OnUpdate()
{
}

void InputManager::ProcessKeyTyped(int32 character)
{
    KeyTypedEvent event((wchar)character);
    keyTypedEventHandler(event);
}

void InputManager::ProcessKeyDown(int32 key)
{
    bool repeat = false;
    key = TranslateKey(key);
    if (key != CT_KEY_UNKNOWN)
    {
        repeat = pressedKeys[key];
        pressedKeys[key] = true;
    }

    KeyDownEvent event(key, repeat);
    keyDownEventHandler(event);
}

void InputManager::ProcessKeyUp(int32 key)
{
    key = TranslateKey(key);
    if (key != CT_KEY_UNKNOWN)
        pressedKeys[key] = false;

    KeyUpEvent event(key);
    keyUpEventHandler(event);
}

void InputManager::ProcessTouchDown(int32 button)
{
    if (button < 0 || button > CT_BUTTON_LAST)
        button = CT_BUTTON_UNKNOWN;

    if (button != CT_BUTTON_UNKNOWN)
        pressedButtons[button] = true;

    TouchDownEvent event(button);
    touchDownEventHandler(event);
}

void InputManager::ProcessTouchUp(int32 button)
{
    if (button < 0 || button > CT_BUTTON_LAST)
        button = CT_BUTTON_UNKNOWN;

    if (button != CT_BUTTON_UNKNOWN)
        pressedButtons[button] = false;

    TouchUpEvent event(button);
    touchUpEventHandler(event);
}

void InputManager::ProcessMouseMoved(int32 posX, int32 posY)
{
    deltaX = posX - mouseX;
    deltaY = posY - mouseY;
    mouseX = posX;
    mouseY = posY;

    MouseMovedEvent event(mouseX, mouseY);
    mouseMovedEventHandler(event);
}

void InputManager::ProcessMouseScrolled(int32 amount)
{
    MouseScrolledEvent event(amount);
    mouseScrolledEventHandler(event);
}
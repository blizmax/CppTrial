#pragma once

#include "Application/.Package.h"

enum class InputEventType
{
    Unknown,

    KeyTyped,
    KeyDown,
    KeyUp,
    TouchDown,
    TouchUp,
    TouchDragged,
    MouseMoved,
    MoveScrolled,
}; 

class InputEvent
{
public:
    InputEventType type = InputEventType::Unknown;

    virtual String GetName() const = 0;
};

class KeyTypedEvent : public InputEvent
{
public:
    char32 character;

    KeyTypedEvent(char32 character) : character(character)
    {
    }

    String GetName() const override
    {
        return CT_TEXT("KeyTypedEvent");
    }
};


class KeyDownEvent : public InputEvent
{
public:
    int32 key;
    int32 repeatCount;

    KeyDownEvent(int32 key, int32 repeatCount) : key(key), repeatCount(repeatCount)
    {
    }

    String GetName() const override
    {
        return CT_TEXT("KeyDownEvent");
    }
};

class KeyUpEvent : public InputEvent
{
public:
    int32 key;

    KeyUpEvent(int32 key) : key(key)
    {
    }

    String GetName() const override
    {
        return CT_TEXT("KeyUpEvent");
    }
};

class TouchDownEvent : public InputEvent
{
public:
    int32 button;

    TouchDownEvent(int32 button) : button(button)
    {
    }

    String GetName() const override
    {
        return CT_TEXT("TouchDownEvent");
    }
};

class TouchUpEvent : public InputEvent
{
public:
    int32 button;

    TouchUpEvent(int32 button) : button(button)
    {
    }

    String GetName() const override
    {
        return CT_TEXT("TouchUpEvent");
    }
};

class MouseMovedEvent : public InputEvent
{
public:
    int32 posX;
    int32 posY;

    MouseMovedEvent(int32 posX, int32 posY) : posX(posX), posY(posY)
    {
    }

    String GetName() const override
    {
        return CT_TEXT("MouseMovedEvent");
    }
};

class MouseScrolledEvent : public InputEvent
{
public:
    int32 amount;

    MouseMovedEvent(int32 amount) : amount(amount)
    {
    }

    String GetName() const override
    {
        return CT_TEXT("MouseScrolledEvent");
    }
};
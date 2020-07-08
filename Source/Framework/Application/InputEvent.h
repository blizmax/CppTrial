#pragma once

#include "Application/.Package.h"
#include "Utils/Event.h"

#define CT_BUTTON_UNKNOWN -1
#define CT_BUTTON_0 0
#define CT_BUTTON_1 1
#define CT_BUTTON_2 2
#define CT_BUTTON_3 3
#define CT_BUTTON_4 4
#define CT_BUTTON_5 5
#define CT_BUTTON_6 6
#define CT_BUTTON_7 7
#define CT_BUTTON_LEFT CT_BUTTON_0
#define CT_BUTTON_RIGHT CT_BUTTON_1
#define CT_BUTTON_MIDDLE CT_BUTTON_2
#define CT_BUTTON_LAST CT_BUTTON_7

#define CT_KEY_UNKNOWN -1
#define CT_KEY_SPACE 32
#define CT_KEY_APOSTROPHE 39 /* ' */
#define CT_KEY_COMMA 44      /* , */
#define CT_KEY_MINUS 45      /* - */
#define CT_KEY_PERIOD 46     /* . */
#define CT_KEY_SLASH 47      /* / */
#define CT_KEY_0 48
#define CT_KEY_1 49
#define CT_KEY_2 50
#define CT_KEY_3 51
#define CT_KEY_4 52
#define CT_KEY_5 53
#define CT_KEY_6 54
#define CT_KEY_7 55
#define CT_KEY_8 56
#define CT_KEY_9 57
#define CT_KEY_SEMICOLON 59 /* ; */
#define CT_KEY_EQUAL 61     /* = */
#define CT_KEY_A 65
#define CT_KEY_B 66
#define CT_KEY_C 67
#define CT_KEY_D 68
#define CT_KEY_E 69
#define CT_KEY_F 70
#define CT_KEY_G 71
#define CT_KEY_H 72
#define CT_KEY_I 73
#define CT_KEY_J 74
#define CT_KEY_K 75
#define CT_KEY_L 76
#define CT_KEY_M 77
#define CT_KEY_N 78
#define CT_KEY_O 79
#define CT_KEY_P 80
#define CT_KEY_Q 81
#define CT_KEY_R 82
#define CT_KEY_S 83
#define CT_KEY_T 84
#define CT_KEY_U 85
#define CT_KEY_V 86
#define CT_KEY_W 87
#define CT_KEY_X 88
#define CT_KEY_Y 89
#define CT_KEY_Z 90
#define CT_KEY_LEFT_BRACKET 91  /* [ */
#define CT_KEY_BACKSLASH 92     /* \ */
#define CT_KEY_RIGHT_BRACKET 93 /* ] */
#define CT_KEY_GRAVE_ACCENT 96  /* ` */
#define CT_KEY_WORLD_1 161      /* non-US #1 */
#define CT_KEY_WORLD_2 162      /* non-US #2 */

#define CT_KEY_ESCAPE 256
#define CT_KEY_ENTER 257
#define CT_KEY_TAB 258
#define CT_KEY_BACKSPACE 259
#define CT_KEY_INSERT 260
#define CT_KEY_DELETE 261
#define CT_KEY_RIGHT 262
#define CT_KEY_LEFT 263
#define CT_KEY_DOWN 264
#define CT_KEY_UP 265
#define CT_KEY_PAGE_UP 266
#define CT_KEY_PAGE_DOWN 267
#define CT_KEY_HOME 268
#define CT_KEY_END 269
#define CT_KEY_CAPS_LOCK 280
#define CT_KEY_SCROLL_LOCK 281
#define CT_KEY_NUM_LOCK 282
#define CT_KEY_PRINT_SCREEN 283
#define CT_KEY_PAUSE 284
#define CT_KEY_F1 290
#define CT_KEY_F2 291
#define CT_KEY_F3 292
#define CT_KEY_F4 293
#define CT_KEY_F5 294
#define CT_KEY_F6 295
#define CT_KEY_F7 296
#define CT_KEY_F8 297
#define CT_KEY_F9 298
#define CT_KEY_F10 299
#define CT_KEY_F11 300
#define CT_KEY_F12 301
#define CT_KEY_F13 302
#define CT_KEY_F14 303
#define CT_KEY_F15 304
#define CT_KEY_F16 305
#define CT_KEY_F17 306
#define CT_KEY_F18 307
#define CT_KEY_F19 308
#define CT_KEY_F20 309
#define CT_KEY_F21 310
#define CT_KEY_F22 311
#define CT_KEY_F23 312
#define CT_KEY_F24 313
#define CT_KEY_F25 314
#define CT_KEY_KP_0 320
#define CT_KEY_KP_1 321
#define CT_KEY_KP_2 322
#define CT_KEY_KP_3 323
#define CT_KEY_KP_4 324
#define CT_KEY_KP_5 325
#define CT_KEY_KP_6 326
#define CT_KEY_KP_7 327
#define CT_KEY_KP_8 328
#define CT_KEY_KP_9 329
#define CT_KEY_KP_DECIMAL 330
#define CT_KEY_KP_DIVIDE 331
#define CT_KEY_KP_MULTIPLY 332
#define CT_KEY_KP_SUBTRACT 333
#define CT_KEY_KP_ADD 334
#define CT_KEY_KP_ENTER 335
#define CT_KEY_KP_EQUAL 336
#define CT_KEY_LEFT_SHIFT 340
#define CT_KEY_LEFT_CONTROL 341
#define CT_KEY_LEFT_ALT 342
#define CT_KEY_LEFT_SUPER 343
#define CT_KEY_RIGHT_SHIFT 344
#define CT_KEY_RIGHT_CONTROL 345
#define CT_KEY_RIGHT_ALT 346
#define CT_KEY_RIGHT_SUPER 347
#define CT_KEY_MENU 348

#define CT_KEY_LAST CT_KEY_MENU

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
    MouseScrolled,
};

class InputEvent : public Event
{
public:
    virtual InputEventType GetEventType() const = 0;
    virtual String GetName() const = 0;
    virtual String ToString() const = 0;
};

class KeyTypedEvent : public InputEvent
{
public:
    static constexpr InputEventType eventType = InputEventType::KeyTyped;
    wchar character;

    KeyTypedEvent(wchar character) : character(character)
    {
    }

    InputEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("KeyTypedEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, character:{1}]"), GetName(), character);
    }
};

class KeyDownEvent : public InputEvent
{
public:
    static constexpr InputEventType eventType = InputEventType::KeyDown;
    int32 key;
    bool repeat;

    KeyDownEvent(int32 key, bool repeat) : key(key), repeat(repeat)
    {
    }

    InputEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("KeyDownEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, key:{1}, repeat:{2}]"), GetName(), key, repeat);
    }
};

class KeyUpEvent : public InputEvent
{
public:
    static constexpr InputEventType eventType = InputEventType::KeyUp;
    int32 key;

    KeyUpEvent(int32 key) : key(key)
    {
    }

    InputEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("KeyUpEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, key:{1}]"), GetName(), key);
    }
};

class TouchDownEvent : public InputEvent
{
public:
    static constexpr InputEventType eventType = InputEventType::TouchDown;
    int32 x;
    int32 y;
    int32 button;

    TouchDownEvent(int32 x, int32 y, int32 button) : x(x), y(y), button(button)
    {
    }

    InputEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("TouchDownEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, button:{1}]"), GetName(), button);
    }
};

class TouchUpEvent : public InputEvent
{
public:
    static constexpr InputEventType eventType = InputEventType::TouchUp;
    int32 x;
    int32 y;
    int32 button;

    TouchUpEvent(int32 x, int32 y, int32 button) : x(x), y(y), button(button)
    {
    }

    InputEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("TouchUpEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, button:{1}]"), GetName(), button);
    }
};

class MouseMovedEvent : public InputEvent
{
public:
    static constexpr InputEventType eventType = InputEventType::MouseMoved;
    int32 x;
    int32 y;

    MouseMovedEvent(int32 x, int32 y) : x(x), y(y)
    {
    }

    InputEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("MouseMovedEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, x:{1}, y:{2}]"), GetName(), x, y);
    }
};

class MouseScrolledEvent : public InputEvent
{
public:
    static constexpr InputEventType eventType = InputEventType::MouseScrolled;
    int32 amount;

    MouseScrolledEvent(int32 amount) : amount(amount)
    {
    }

    InputEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("MouseScrolledEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, amount]"), GetName(), amount);
    }
};
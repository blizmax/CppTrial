#pragma once

#include "Application/.Package.h"

enum class WindowEventType
{
    Unknown,

    WindowResize,
    WindowFocus,
    FilesDropped,
};

class WindowEvent
{
public:
    virtual WindowEventType GetEventType() const = 0;
    virtual String GetName() const = 0;
    virtual String ToString() const = 0;
};

class WindowResizeEvent : public WindowEvent
{
public:
    static constexpr WindowEventType eventType = WindowEventType::WindowResize;
    uint32 width;
    uint32 height;

    WindowResizeEvent(uint32 width, uint32 height) : width(width), height(height)
    {
    }

    WindowEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("WindowResizeEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, width:{1}, height:{2}]"), GetName(), width, height);
    }
};

class WindowFocusEvent : public WindowEvent
{
public:
    static constexpr WindowEventType eventType = WindowEventType::WindowFocus;
    bool focused;

    WindowFocusEvent(bool focused) : focused(focused)
    {
    }

    WindowEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("WindowFocusEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, focused:{1}]"), GetName(), focused);
    }
};

class FilesDroppedEvent : public WindowEvent
{
public:
    static constexpr WindowEventType eventType = WindowEventType::FilesDropped;
    Array<String> paths;

    FilesDroppedEvent(const Array<String> &paths) : paths(paths)
    {
    }

    FilesDroppedEvent(Array<String> &&paths) : paths(std::move(paths))
    {
    }

    WindowEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("FilesDroppedEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}]"), GetName());
    }
};
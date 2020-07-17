#pragma once

#include "Application/.Package.h"
#include "Utils/Event.h"

enum class WindowEventType
{
    Unknown,

    WindowResized,
    FocusGained,
    FocusLost,
    FilesDropped,
};

class WindowEvent : public Event
{
public:
    virtual WindowEventType GetEventType() const = 0;
    virtual String GetName() const = 0;
    virtual String ToString() const = 0;
};

class WindowResizedEvent : public WindowEvent
{
public:
    static constexpr WindowEventType eventType = WindowEventType::WindowResized;
    int32 width;
    int32 height;

    WindowResizedEvent(int32 width, int32 height)
        : width(width), height(height)
    {
    }

    WindowEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("WindowResizedEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, width:{1}, height:{2}]"), GetName(), width, height);
    }
};

class FocusGainedEvent : public WindowEvent
{
public:
    static constexpr WindowEventType eventType = WindowEventType::FocusGained;

    FocusGainedEvent()
    {
    }

    WindowEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("FocusGainedEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, focused:{1}]"), GetName(), true);
    }
};

class FocusLostEvent : public WindowEvent
{
public:
    static constexpr WindowEventType eventType = WindowEventType::FocusLost;

    FocusLostEvent()
    {
    }

    WindowEventType GetEventType() const override
    {
        return eventType;
    }

    String GetName() const override
    {
        return CT_TEXT("FocusLostEvent");
    }

    String ToString() const override
    {
        return String::Format(CT_TEXT("[name:{0}, focused:{1}]"), GetName(), false);
    }
};

class FilesDroppedEvent : public WindowEvent
{
public:
    static constexpr WindowEventType eventType = WindowEventType::FilesDropped;
    Array<String> paths;

    FilesDroppedEvent(const Array<String> &paths)
        : paths(paths)
    {
    }

    FilesDroppedEvent(Array<String> &&paths)
        : paths(std::move(paths))
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
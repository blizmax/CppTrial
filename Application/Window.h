#pragma once

#include "Application/.Package.h"
#include "Application/Input.h"
#include "Application/WindowEvent.h"
#include "Utils/Module.h"

struct WindowConfig
{
    String title;
    uint32 width;
    uint32 height;
};

class Window : public Module
{
public:
    Window() = default;
    virtual ~Window() = default;

public:
    using WindowResizedEventDelegate = Delegate<void(WindowResizedEvent &)>;
    using FocusGainedEventDelegate = Delegate<void(FocusGainedEvent &)>;
    using FocusLostEventDelegate = Delegate<void(FocusLostEvent &)>;
    using FileDroppedEventDelegate = Delegate<void(FilesDroppedEvent &)>;

    WindowResizedEventDelegate windowResizedHandler;
    FocusGainedEventDelegate focusGainedHandler;
    FocusLostEventDelegate focusLostHandler;
    FileDroppedEventDelegate fileDroppedHandler;

    virtual Input &GetInput() = 0;

    virtual uint32 GetPositionX() const = 0;
    virtual uint32 GetPositionY() const = 0;
    virtual uint32 GetWidth() const = 0;
    virtual uint32 GetHeight() const = 0;

    virtual void SetTitle(const String &title) = 0;

    virtual void Minimize() = 0;
    virtual void Maximize() = 0;
    virtual void Restore() = 0;
    virtual void Show() = 0;
    virtual void Hide() = 0;
    virtual void Focus() = 0;
    virtual void Flash() = 0;

    virtual bool Minimized() const = 0;
    virtual bool Maximized() const = 0;
    virtual bool Visible() const = 0;
    virtual bool Focused() const = 0;

    virtual void CreateNativeWindow(const WindowConfig &config) = 0;
    virtual void *GetNativeHandle() const = 0;

    virtual String GetName() const override
    {
        return CT_TEXT("Window");
    }

public:
    static Window *Create(const WindowConfig &config);
};
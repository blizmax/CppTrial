#pragma once

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
    Window() : Module(CT_TEXT("Window")) {}
    virtual ~Window() = default;

    virtual uint32 GetWidth() const = 0;
    virtual uint32 GetHeight() const = 0;

    virtual void CreateNativeWindow(const WindowConfig &config) = 0;
    virtual void *GetNativeHandler() const = 0;

public:
    static Window *Create(const WindowConfig &config);
};
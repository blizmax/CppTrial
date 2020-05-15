#pragma once

#include "Application/Windows/WindowsWindow.h"

class GLWindowsWindow : public WindowsWindow
{
public:
    virtual void Shutdown() override;
    virtual void Tick() override;
    virtual void CreateNativeWindow(const WindowDesc &desc) override;

private:
    HDC hdc;
    HGLRC renderContext;
    PIXELFORMATDESCRIPTOR pfd;
    int pixelFormat;
};
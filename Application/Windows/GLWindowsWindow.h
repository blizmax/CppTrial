#pragma once

#include "Application/Windows/WindowsWindow.h"

class GLWindowsWindow : public WindowsWindow
{
public:
    virtual void OnUnload() override;
    virtual void OnTick() override;
    virtual void CreateNativeWindow(const WindowConfig &config) override;

private:
    HDC hdc;
    HGLRC renderContext;
    PIXELFORMATDESCRIPTOR pfd;
    int pixelFormat;
};
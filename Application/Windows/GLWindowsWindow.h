#pragma once

#include "Application/Windows/WindowsWindow.h"

class GLWindowsWindow : public WindowsWindow
{
public:
    virtual void OnDetach() override;
    virtual void CreateNativeWindow(const WindowConfig &config) override;

private:
    HDC hdc;
    HGLRC renderContext;
    PIXELFORMATDESCRIPTOR pfd;
    int pixelFormat;
};
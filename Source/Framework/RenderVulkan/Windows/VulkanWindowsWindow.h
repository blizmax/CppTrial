#pragma once

#include "Application/Windows/WindowsWindow.h"

class VulkanWindowsWindow : public WindowsWindow
{
public:
    virtual void Shutdown() override;
    virtual void Tick() override;
    virtual void CreateNativeWindow(const WindowConfig &config) override;

private:
    HDC hdc;
    HGLRC renderContext;
    PIXELFORMATDESCRIPTOR pfd;
    int pixelFormat;
};
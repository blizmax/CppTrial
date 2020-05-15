#pragma once

#include "Application/Desktop/DesktopWindow.h"

class GLDesktopWindow : public DesktopWindow
{
public:
    virtual void Tick() override;
    virtual void CreateNativeWindow(const WindowDesc &desc) override;
};
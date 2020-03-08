#pragma once

#include "Application/.Package.h"
#include "Application/Window.h"
#include "Application/Logic.h"
#include "Application/Input.h"

class Application
{
public:
    Application() = default;
    virtual ~Application() = default;

    virtual void PreInit(const WindowConfig &config);
    virtual void Init();
    virtual void Run();
    virtual void Exit();

    Input &GetInput();

    Window &GetWindow()
    {
        return *window;
    }

    void RequestQuit()
    {
        requestingQuit = true;
    }

private:
    Window *window;
    Logic *logic;
    bool requestingQuit = false;
};

extern Application *gApp;
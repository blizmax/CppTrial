#pragma once

#include "Application/.Package.h"
#include "Application/Window.h"
#include "Application/Logic.h"
#include "Application/Input.h"
#include "Application/Clipboard.h"

class Application
{
public:
    Application() = default;
    virtual ~Application() = default;

    virtual void PreInit(const WindowDesc &desc);
    virtual void Init();
    virtual void Run();
    virtual void Exit();

    Input &GetInput();
    Clipboard &GetClipboard();

    int32 GetFramesPerSecond() const
    {
        return fps;
    }

    int32 GetTotalFrames() const
    {
        return totalFrames;
    }

    int64 GetStartTime() const
    {
        return startTime;
    }

    float GetDeltaTime() const
    {
        return deltaTime;
    }  

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
    float deltaTime;
    int64 startTime = 0;
    int64 lastTime = 0;
    int64 frameCountTime = 0;
    int32 totalFrames = 0;
    int32 frames = 0;
    int32 fps = 0;
};

extern Application *gApp;
#include "Application/Application.h"
#include "Render/RenderManager.h"
#include "Core/Time.h"

#include "Application/ImGuiLab.h"

#if CT_DEBUG
#include "Application/DebugManager.h"
#endif

void Application::PreInit(const WindowConfig &config)
{
    auto time = Time::NanoTime();
    startTime = time;
    frameCountTime = time;

#if CT_DEBUG
    gDebugManager->OnLoad();
    CT_LOG(Info, CT_TEXT("DebugManager loaded."));
#endif
    window = Window::Create(config);
    CT_LOG(Info, CT_TEXT("Primary window created."));
    
    Input::Init();
    Clipboard::Init();
}

void Application::Init()
{
    gRenderManager->OnLoad();
    CT_LOG(Info, CT_TEXT("RenderManager loaded."));

    gImGuiLab->OnLoad();
    CT_LOG(Info, CT_TEXT("ImGuiLab loaded."));

    gLogic->OnLoad();
    CT_LOG(Info, CT_TEXT("Logic loaded."));
}

void Application::Run()
{
    while (!requestingQuit)
    {
        auto time = Time::NanoTime();
        deltaTime = (time - lastTime) / 1000000000.0f;
        lastTime = time;
        if(time - frameCountTime >= 1000000000)
        {
            frameCountTime = time;
            fps = frames;
            frames = 0;
        }
        ++frames;
        ++totalFrames;

        CT_PROFILE_SESSION_BEGIN(CT_TEXT("Logic"));
        gLogic->OnUpdate();

        CT_PROFILE_SESSION_BEGIN(CT_TEXT("RenderManager"));
        gRenderManager->OnUpdate();

        CT_PROFILE_SESSION_BEGIN(CT_TEXT("ImGuiLab"));
        gImGuiLab->OnUpdate();

#if CT_DEBUG
        gDebugManager->OnUpdate();
#endif

        window->OnUpdate();
    }
}

void Application::Exit()
{
    gLogic->OnUnload();

    gImGuiLab->OnUnload();

    gRenderManager->OnUnload();

#if CT_DEBUG
    gDebugManager->OnUnload();
#endif
}

Input &Application::GetInput()
{
    return window->GetInput();
}

Clipboard &Application::GetClipboard()
{
    return *gClipboard;
}
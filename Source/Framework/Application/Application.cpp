#include "Application/Application.h"
#include "Application/ThreadManager.h"
#include "Render/RenderManager.h"
#include "Assets/AssetManager.h"
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
    gDebugManager->Startup();
    CT_LOG(Info, CT_TEXT("DebugManager startup."));
#endif

    gThreadManager->Startup();
    CT_LOG(Info, CT_TEXT("ThreadManager startup."));

    window = Window::Create(config);
    CT_LOG(Info, CT_TEXT("Primary window created."));
    
    Input::Init();
    Clipboard::Init();
}

void Application::Init()
{
    gRenderManager->Startup();
    CT_LOG(Info, CT_TEXT("RenderManager startup."));

    gAssetManager->Startup();
    CT_LOG(Info, CT_TEXT("AssetManager startup."));

    gImGuiLab->Startup();
    CT_LOG(Info, CT_TEXT("ImGuiLab startup."));

    gLogic->Startup();
    CT_LOG(Info, CT_TEXT("Logic startup."));
}

void Application::Run()
{
    while (!requestingQuit)
    {
        auto time = Time::NanoTime();
        deltaTime = lastTime > 0 ? (time - lastTime) / 1000000000.0f : 0.0f;
        lastTime = time;
        if(time - frameCountTime >= 1000000000)
        {
            frameCountTime = time;
            fps = frames;
            frames = 0;
        }
        ++frames;
        ++totalFrames;

        gThreadManager->Tick();

        CT_PROFILE_SESSION_BEGIN(CT_TEXT("Logic"));
        gLogic->Tick();

        CT_PROFILE_SESSION_BEGIN(CT_TEXT("RenderManager"));
        gRenderManager->Tick();

        CT_PROFILE_SESSION_BEGIN(CT_TEXT("ImGuiLab"));
        gImGuiLab->Tick();

#if CT_DEBUG
        gDebugManager->Tick();
#endif

        window->Tick();
    }
}

void Application::Exit()
{
    gLogic->Shutdown();
    CT_LOG(Info, CT_TEXT("Logic shutdown."));

    gImGuiLab->Shutdown();
    CT_LOG(Info, CT_TEXT("ImGui shutdown."));

    gRenderManager->Shutdown();
    CT_LOG(Info, CT_TEXT("RenderManager shutdown."));

    gThreadManager->Shutdown();
    CT_LOG(Info, CT_TEXT("ThreadManager shutdown."));

#if CT_DEBUG
    gDebugManager->Shutdown();
    CT_LOG(Info, CT_TEXT("DebugManager shutdown."));
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
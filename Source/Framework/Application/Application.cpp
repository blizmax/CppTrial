#include "Application/Application.h"
#include "Application/ThreadManager.h"
#include "Render/RenderManager.h"
#include "Assets/AssetManager.h"
#include "Core/Time.h"

#include "Application/ImGuiLab.h"

#if CT_DEBUG
#include "Application/DebugManager.h"
#endif

void Application::PreInit(const WindowDesc &desc)
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

    Window::Init();
    window = Window::Create(desc);
    window->Startup();
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
    GetInput().keyTypedHandler += ([](auto &e) {
        if (e.handled)
            return;
        gLogic->OnKeyTyped(e);
    });
    GetInput().keyDownHandler += ([](auto &e) {
        if (e.handled)
            return;
        gLogic->OnKeyDown(e);
    });
    GetInput().keyUpHandler += ([](auto &e) {
        if (e.handled)
            return;
        gLogic->OnKeyUp(e);
    });
    GetInput().touchDownHandler += ([](auto &e) {
        if (e.handled)
            return;
        gLogic->OnTouchDown(e);
    });
    GetInput().touchUpHandler += ([](auto &e) {
        if (e.handled)
            return;
        gLogic->OnTouchUp(e);
    });
    GetInput().mouseMovedHandler += ([](auto &e) {
        if (e.handled)
            return;
        gLogic->OnMouseMoved(e);
    });
    GetInput().mouseScrolledHandler += ([](auto &e) {
        if (e.handled)
            return;
        gLogic->OnMouseScrolled(e);
    });

    CT_LOG(Info, CT_TEXT("Logic startup."));
}

void Application::Run()
{
    while (!requestingQuit)
    {
        auto time = Time::NanoTime();
        auto delta = time - lastTime < 100 ? 100 : time - lastTime; //TODO Sleep?
        deltaTime = lastTime > 0 ? delta / 1000000000.0f : 0.0f;
        lastTime = time;
        if (time - frameCountTime >= 1000000000)
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

        gRenderManager->Present();

        if (window->ShouldClose())
            RequestQuit();
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

    window->Shutdown();
    CT_LOG(Info, CT_TEXT("Primary window shutdown."));

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
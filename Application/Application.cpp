#include "Application/Application.h"
#include "Application/InputManager.h"
#include "Render/RenderManager.h"

#if CT_DEBUG
#include "Application/DebugManager.h"
#endif

void Application::PreInit(const WindowConfig &config)
{
#if CT_DEBUG
    gDebugManager->OnLoad();
    CT_LOG(Info, CT_TEXT("DebugManager loaded."));
#endif
    window = Window::Create(config);
    CT_LOG(Info, CT_TEXT("Primary window created."));
}

void Application::Init()
{
    gInputManager->OnLoad();
    CT_LOG(Info, CT_TEXT("InputManager loaded."));

    gRenderManager->OnLoad();
    CT_LOG(Info, CT_TEXT("RenderManager loaded."));

    gLogic->OnLoad();
    CT_LOG(Info, CT_TEXT("Logic loaded."));
}

void Application::Run()
{
    while (!requestingQuit)
    {
        gInputManager->OnUpdate();

        gLogic->OnUpdate();

        gRenderManager->OnUpdate();

#if CT_DEBUG
        gDebugManager->OnUpdate();
#endif

        window->OnUpdate();
    }
}

void Application::Exit()
{
    gLogic->OnUnload();

    gRenderManager->OnUnload();
    gInputManager->OnUnload();

#if CT_DEBUG
    gDebugManager->OnUnload();
#endif
}

Input &Application::GetInput()
{
    return gInputManager->GetInput();
}
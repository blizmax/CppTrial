#include "Application/Application.h"
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
    
    GetInput().Init();
}

void Application::Init()
{
    gRenderManager->OnLoad();
    CT_LOG(Info, CT_TEXT("RenderManager loaded."));

    gLogic->OnLoad();
    CT_LOG(Info, CT_TEXT("Logic loaded."));
}

void Application::Run()
{
    while (!requestingQuit)
    {
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

#if CT_DEBUG
    gDebugManager->OnUnload();
#endif
}

Input &Application::GetInput()
{
    return window->GetInput();
}
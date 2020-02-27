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
#endif

    window = Window::Create(config);
}

void Application::Init()
{
    gInputManager->OnLoad();
    gRenderManager->OnLoad();
}

void Application::Run()
{
    while (!requestingQuit)
    {

#if CT_DEBUG
        gDebugManager->OnUpdate();
#endif

        window->OnUpdate();

        gInputManager->OnUpdate();
        gRenderManager->OnUpdate();
    }
}

void Application::Exit()
{
    gRenderManager->OnUnload();
    gInputManager->OnUnload();

#if CT_DEBUG
    gDebugManager->OnUnload();
#endif
}
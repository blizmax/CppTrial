#include "Application/Application.h"
#include "Application/InputManager.h"
#include "Render/RenderManager.h"

#if CT_DEBUG
#include "Application/DebugManager.h"
#endif

void Application::PreInit(const WindowConfig &config)
{
    window = Window::Create(config);
}

void Application::Init()
{
    gInputManager->OnLoad();
    gRenderManager->OnLoad();

#if CT_DEBUG
    gDebugManager->OnLoad();
#endif
}

void Application::Run()
{
    while (!requestingQuit)
    {
        gInputManager->OnUpdate();
        gRenderManager->OnUpdate();

#if CT_DEBUG
        gDebugManager->OnUpdate();
#endif

        window->OnUpdate();
    }
}

void Application::Exit()
{
#if CT_DEBUG
    gDebugManager->OnUnload();
#endif

    gRenderManager->OnUnload();
    gInputManager->OnUnload();
}
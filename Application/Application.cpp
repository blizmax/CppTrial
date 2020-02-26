#include "Application/Application.h"
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

    gRenderManager->OnLoad();
}

void Application::Init()
{
}

void Application::Run()
{
    while (!requestingQuit)
    {

#if CT_DEBUG
        gDebugManager->OnUpdate();
#endif

        window->OnUpdate();

        gRenderManager->OnUpdate();
    }
}

void Application::Exit()
{
    gRenderManager->OnUnload();

#if CT_DEBUG
    gDebugManager->OnUnload();
#endif
}
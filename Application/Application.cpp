#include "Application/Application.h"

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
}

void Application::Run()
{
    while (!requestingQuit)
    {

#if CT_DEBUG
        gDebugManager->OnTick();
#endif

        window->OnTick();
    }
}

void Application::Exit()
{
#if CT_DEBUG
    gDebugManager->OnUnload();
#endif
}
#include "Application/DebugManager.h"

#if CT_DEBUG

#include "Render/RenderAPI.h"

DebugManager debugManager;
DebugManager *gDebugManager = &debugManager;


void DebugManager::OnLoad()
{
     CT_LOG(Debug, CT_TEXT("DebugManager initialized."));
}

void DebugManager::OnUnload()
{

}

void DebugManager::OnUpdate()
{
    RenderAPI::SetClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    RenderAPI::Clear();
}

#endif
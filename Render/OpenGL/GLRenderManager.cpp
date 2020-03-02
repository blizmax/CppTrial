#include "Render/OpenGL/GLRenderManager.h"

GLRenderManager renderManager;
RenderManager *gRenderManager = &renderManager;

void GLRenderManager::OnLoad()
{
    if (!gladLoadGL()) 
    {
        CT_EXCEPTION(Init, "gladLoadGL failed.");
        return;
    }

	CT_LOG(Debug, CT_TEXT("GL Vendor:  {0}"), String(reinterpret_cast<const char *>(glGetString(GL_VENDOR))));
	CT_LOG(Debug, CT_TEXT("GL Renderer: {0}"), String(reinterpret_cast<const char *>(glGetString(GL_RENDERER))));
	CT_LOG(Debug, CT_TEXT("GL Version: {0}"), String(reinterpret_cast<const char *>(glGetString(GL_VERSION))));

    CT_LOG(Info, CT_TEXT("RenderManager loaded."));
}
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

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
}

void GLRenderManager::OnUpdate()
{
}
#include "RenderOpenGL/.Package.h"
#include "Render/RenderAPI.h"

void RenderAPI::SetViewport(int32 x, int32 y, uint32 width, uint32 height)
{
    glViewport(x, y, width, height);
}

void RenderAPI::SetScissor(int32 x, int32 y, uint32 width, uint32 height)
{
    glScissor(x, y, width, height);
}

void RenderAPI::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void RenderAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderAPI::DrawIndexed(const SPtr<VertexArray> &vertexArray)
{
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void RenderAPI::DrawIndexed(uint32 startIndex, uint32 indexCount)
{
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, reinterpret_cast<void *>((uint64)startIndex * sizeof(uint32)));
}
#pragma once

#include "Render/VertexArray.h"

class RenderAPI
{
public:
    static void SetViewport(int32 x, int32 y, uint32 width, uint32 height);
    static void SetScissor(int32 x, int32 y, uint32 width, uint32 height);

    static void SetClearColor(float r, float g, float b, float a);
    static void SetClearColor(const Color &color) { SetClearColor(color.r, color.g, color.b, color.a); }
    static void Clear();

    static void DrawIndexed(const SPtr<VertexArray> &vertexArray);
    static void DrawIndexed(uint32 startIndex, uint32 indexCount);
};
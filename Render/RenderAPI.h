#pragma once

#include "Render/VertexArray.h"

class RenderAPI
{
public:
    static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
    static void SetClearColor(float r, float g, float b, float a);
    static void Clear();

    static void DrawIndexed(const SPtr<VertexArray> &vertexArray);
};
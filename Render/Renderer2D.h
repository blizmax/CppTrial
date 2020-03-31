#pragma once

#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"

class Renderer2D
{
public:
    Renderer2D();

    void Begin();
    void End();
    void Flush();

    bool IsDrawing() const
    {
        return drawing;
    }

    int32 GetDrawCalls() const
    {
        return drawCalls;
    }

    void SetViewProjectionMatrix(const Matrix4 &mat);
    Matrix4 GetViewProjectionMatrix() const;
    void SetColor(const Color &tint);
    Color GetColor() const;

    void DrawLine(float x1, float y1, float x2, float y2, float thickness);
    void DrawRect(float x, float y, float width, float height, float rounding, float thickness);
    void DrawRectFilled(float x, float y, float width, float height, float rounding);
    void DrawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float thickness);
    void DrawQuadFilled(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
    void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float thickness);
    void DrawTriangleFilled(float x1, float y1, float x2, float y2, float x3, float y3);
    void DrawCircle(float x, float y, float radius, int32 segments, float thickness);
    void DrawCircleFilled(float x, float y, float radius, int32 segments);
    void DrawBezierCurve(float x1, float y1, float x2, float y2, float px1, float py1, float px2, float py2, float thickness, int32 segments = 0);

private:
    void PrimRect(const SPtr<Texture> &tex, float x, float y, float w, float h, float u1, float v1, float u2, float v2);
    void PrimQuad(const SPtr<Texture> &tex, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4);

    void UpdateUniforms();
    void CheckTexture(const SPtr<Texture> &texture);
    void CheckCapacity(int32 vtxNum);

private:
    static constexpr int32 MAX_VERTICES = 5000;
    static constexpr int32 ELEMENTS_PER_VERTEX = 5;

    Array<float> vertices;
    Array<uint32> indices;

    SPtr<Shader> shader;
    SPtr<VertexArray> vertexArray;
    SPtr<Texture> currentTexture;
    Color color = Color::WHITE;
    Matrix4 viewProjection;

    bool drawing;
    int32 drawCalls;
};
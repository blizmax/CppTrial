#include "Render/Renderer2D.h"

Renderer2D::Renderer2D()
{
    //TODO load shader
    vertexArray = VertexArray::Create();
}

void Renderer2D::SetViewProjectionMatrix(const Matrix4 &mat)
{
    if(drawing)
        Flush();

    viewProjection = mat;

    if(drawing)
        UpdateUniforms();
}

Matrix4 Renderer2D::GetViewProjectionMatrix() const
{
    return viewProjection;
}

void Renderer2D::SetColor(const Color &tint)
{
    color = tint;
}

Color Renderer2D::GetColor() const
{
    return color;
}

void Renderer2D::Begin()
{
    if(drawing)
    {
        CT_EXCEPTION(Render, "End() must be called before.");
        return;
    }

    shader->Bind();
    UpdateUniforms();

    drawCalls = 0;
    drawing = true;
}

void Renderer2D::End()
{
    if(!drawing)
    {
        CT_EXCEPTION(Render, "Begin() must be called before.");
        return;
    }

    Flush();
    shader->Unbind();
    currentTexture = nullptr;

    drawing = false;
}

void Renderer2D::Flush()
{
    if(!drawing)
    {
        CT_EXCEPTION(Render, "Can only call Flush() between Begin() and End().");
        return;
    }

    if(vertices.Count() == 0)
    {
        return;
    }

    ++drawCalls;

    currentTexture->Bind(0);
    //TODO
}

void Renderer2D::CheckTexture(const SPtr<Texture> &texture)
{
    if(!texture)
    {
        CT_EXCEPTION(Render, "Texture can not be null.");
        return;
    }
    if(texture != currentTexture)
    {
        Flush();
        currentTexture = texture;
    }
}

void Renderer2D::CheckCapacity(int32 vtxNum)
{
    if(vtxNum * ELEMENTS_PER_VERTEX + vertices.Count() > MAX_VERTICES)
    {
        Flush();
    }
}

void Renderer2D::UpdateUniforms()
{

}
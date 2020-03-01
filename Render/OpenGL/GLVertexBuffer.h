#pragma once

#include "Render/OpenGL/.Package.h"
#include "Render/VertexBuffer.h"

class GLVertexBuffer : public VertexBuffer
{
public:
    GLVertexBuffer(float *vertices, uint32 size);
    virtual ~GLVertexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetLayout(const VertexBufferLayout &layout) override
    {
        this->layout = layout;
    }

    virtual const VertexBufferLayout &GetLayout() const override
    {
        return layout;
    }

private:
    uint32 id = 0;
    VertexBufferLayout layout;
};
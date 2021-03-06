#pragma once

#include "RenderOpenGL/.Package.h"
#include "Render/VertexBuffer.h"

class GLVertexBuffer : public VertexBuffer
{
public:
    GLVertexBuffer(float *vertices, uint32 size, GpuBufferUsage usage);
    virtual ~GLVertexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SetData(float *vertices, uint32 size) override;

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
    GpuBufferUsage usage;
    VertexBufferLayout layout;
};
#include "RenderOpenGL/GLIndexBuffer.h"

SPtr<IndexBuffer> IndexBuffer::Create(uint32 *indices, uint32 count, GpuBufferUsage usage)
{
    return Memory::MakeShared<GLIndexBuffer>(indices, count, usage);
}

GLIndexBuffer::GLIndexBuffer(uint32 *indices, uint32 count, GpuBufferUsage usage) : usage(usage)
{
    glCreateBuffers(1, &id);
    SetData(indices, count);
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &id);
    id = 0;
}

void GLIndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void GLIndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLIndexBuffer::SetData(uint32 *indices, uint32 count)
{
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32), indices, Render::GetGLUsage(usage));

    this->count = count;

    Unbind();
}
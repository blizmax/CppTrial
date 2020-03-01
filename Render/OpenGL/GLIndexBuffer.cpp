#include "Render/OpenGL/GLIndexBuffer.h"

SPtr<IndexBuffer> IndexBuffer::Create(uint32 *indices, uint32 count)
{
    return Memory::MakeShared<GLIndexBuffer>(indices, count);
}

GLIndexBuffer::GLIndexBuffer(uint32 *indices, uint32 count) : count(count)
{
    glCreateBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32), indices, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &id);
    id = 0;
}

void GLIndexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void GLIndexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
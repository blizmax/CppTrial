#include "Graphics/OpenGL/GLVertexBuffer.h"

SPtr<VertexBuffer> VertexBuffer::Create(float *vertices, uint32 size)
{
    return Memory::MakeShared<GLVertexBuffer>(vertices, size);
}

GLVertexBuffer::GLVertexBuffer(float *vertices, uint32 size)
{
    glCreateBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &id);
    id = 0;
}

void GLVertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void GLVertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
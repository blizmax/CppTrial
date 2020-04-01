#include "Render/OpenGL/GLVertexBuffer.h"
#include "Render/OpenGL/GLUtils.h"

SPtr<VertexBuffer> VertexBuffer::Create(float *vertices, uint32 size, GpuBufferUsage usage)
{
    return Memory::MakeShared<GLVertexBuffer>(vertices, size, usage);
}

GLVertexBuffer::GLVertexBuffer(float *vertices, uint32 size, GpuBufferUsage usage) : usage(usage)
{
    glCreateBuffers(1, &id);
    SetData(vertices, size);
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

void GLVertexBuffer::SetData(float *vertices, uint32 size)
{
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GLUtils::GetGLUsage(usage));
}
#include "Render/OpenGL/GLVertexArray.h"

GLVertexArray::GLVertexArray()
{
    glCreateVertexArrays(1, &id);
}

GLVertexArray::~GLVertexArray()
{
    glDeleteVertexArrays(1, &id);
}

void GLVertexArray::Bind() const
{
    glBindVertexArray(id);
}

void GLVertexArray::Unbind() const
{
    glBindVertexArray(0);
}
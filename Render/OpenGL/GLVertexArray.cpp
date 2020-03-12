#include "Render/OpenGL/GLVertexArray.h"

SPtr<VertexArray> VertexArray::Create()
{
    return Memory::MakeShared<GLVertexArray>();
}

static GLenum GetGLTypeByVertexDataType(VertexDataType dataType)
{
    switch (dataType)
    {
    case VertexDataType::Float1:
        return GL_FLOAT;
    case VertexDataType::Float2:
        return GL_FLOAT;
    case VertexDataType::Float3:
        return GL_FLOAT;
    case VertexDataType::Float4:
        return GL_FLOAT;
    case VertexDataType::Int2:
        return GL_INT;
    case VertexDataType::Int3:
        return GL_INT;
    case VertexDataType::Int4:
        return GL_INT;
    case VertexDataType::UByte4:
        return GL_UNSIGNED_BYTE;
    }

    CT_EXCEPTION(Render, "Unknown data type!");
    return 0;
}

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

void GLVertexArray::AddVertexBuffer(const SPtr<VertexBuffer> &buffer)
{
    glBindVertexArray(id);
    buffer->Bind();

    const auto &layout = buffer->GetLayout();
    for (const auto &attr : layout.GetAttributes())
    {
        glEnableVertexAttribArray(vertexBufferIndex);
        glVertexAttribPointer(vertexBufferIndex,
                              attr.GetComponentCount(),
                              GetGLTypeByVertexDataType(attr.dataType),
                              attr.normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(),
                              reinterpret_cast<void *>(attr.offset));

        ++vertexBufferIndex;
    }

    vertexBuffers.Add(buffer);
}

void GLVertexArray::SetIndexBuffer(const SPtr<IndexBuffer> &buffer)
{
    glBindVertexArray(id);
    buffer->Bind();

    indexBuffer = buffer;
}
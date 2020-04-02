#include "Render/OpenGL/GLFrameBuffer.h"

GLFrameBuffer::GLFrameBuffer(uint32 width, uint32 height, bool hasRenderBuffer)
    : width(width), height(height), hasRenderBuffer(hasRenderBuffer)
{
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    //TODO Color attachment

    if(hasRenderBuffer)
    {
        glGenRenderbuffers(1, &renderBufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width,height);
    }
}

GLFrameBuffer::~GLFrameBuffer()
{
    glDeleteFramebuffers(1, &id);
    id = 0;

    if(hasRenderBuffer)
    {
        glDeleteRenderbuffers(1, &renderBufferID);
        renderBufferID = 0;
    }
}

void GLFrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void GLFrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}
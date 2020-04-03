#include "Render/OpenGL/GLFrameBuffer.h"

GLFrameBuffer::GLFrameBuffer(uint32 width, uint32 height, int32 colorAttachNum)
    : width(width), height(height), useRenderBuffer(useRenderBuffer)
{
    glGenFramebuffers(1, &id);
    Bind();

    //Packed render buffer
    glGenRenderbuffers(1, &renderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);

    //Color textures
    Array<uint32> slots;
    for (int32 i = 0; i < colorAttachNum; ++i)
    {
        auto texture = Texture::Create(width, height);
        texture->SetMinFilter(TextureFilter::Linear);
        texture->SetMagFilter(TextureFilter::Linear);
        texture->SetUWrap(TextureWrap::Clamp);
        texture->SetVWrap(TextureWrap::Clamp);

        uint32 slot = GL_COLOR_ATTACHMENT0 + i;
        uint32 textureID = static_cast<uint32>(reinterpret_cast<SizeType>(texture->GetNativeHandle()));
        glFramebufferTexture2D(GL_FRAMEBUFFER, slot, GL_TEXTURE_2D, textureID, 0);

        colorAttachments.Add(texture);
        slots.Add(slot);
    }
    glDrawBuffers(slots.Count(), slots.GetData());

    Unbind();
}

GLFrameBuffer::~GLFrameBuffer()
{
    glDeleteFramebuffers(1, &id);
    id = 0;

    glDeleteRenderbuffers(1, &renderBufferID);
    renderBufferID = 0;
}

void GLFrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void GLFrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}
#pragma once

#include "Render/OpenGL/.Package.h"
#include "Render/Texture.h"

struct GLFrameBufferDesc
{
    uint32 width;
    uint32 height;

};

class GLFrameBuffer
{
public:
    GLFrameBuffer(uint32 width, uint32 height, bool hasRenderBuffer = true);
    ~GLFrameBuffer();

    void Bind() const;
    void Unbind() const;

    void AddColorAttachment();
    void AddDepthAttachment();
    void AddStencilAttachment();

    uint32 GetWidth() const
    {
        return width;
    }

    uint32 GetHeight() const
    {
        return height;
    }

private:

private:
    uint32 id = 0;
    uint32 width;
    uint32 height;

    Array<SPtr<Texture>> attachments;
    uint32 renderBufferID = 0;
    bool hasRenderBuffer;
};
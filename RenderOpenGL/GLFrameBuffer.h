#pragma once

#include "RenderOpenGL/.Package.h"
#include "Render/Texture.h"

class GLFrameBuffer
{
public:

    GLFrameBuffer(uint32 width, uint32 height, int32 colorAttachNum = 1);
    ~GLFrameBuffer();

    void Bind() const;
    void Unbind() const;

    bool IsMRT() const
    {
        return colorAttachments.Count() > 1;
    }

    bool HasColorAttachment(int32 index) const
    {
        return colorAttachments.Count() > index;
    }

    const SPtr<Texture> &GetColorAttachment(int32 index) const
    {
        return colorAttachments[index];
    }

    uint32 GetWidth() const
    {
        return width;
    }

    uint32 GetHeight() const
    {
        return height;
    }

private:
    uint32 id = 0;
    uint32 width;
    uint32 height;

    Array<SPtr<Texture>> colorAttachments;
    uint32 renderBufferID = 0;
    bool useRenderBuffer;
};
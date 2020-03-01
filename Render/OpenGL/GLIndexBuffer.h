#pragma once

#include "Render/OpenGL/.Package.h"
#include "Render/IndexBuffer.h"

class GLIndexBuffer : public IndexBuffer
{
public:
    GLIndexBuffer(uint32 *indices, uint32 count);
    virtual ~GLIndexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual uint32 GetCount() const override
    {
        return count;
    }

private:
    uint32 id = 0;
    uint32 count;
};
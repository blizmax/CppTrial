#pragma once

#include "Graphics/.Package.h"

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static SPtr<VertexBuffer> Create(float *vertices, uint32 size);
};
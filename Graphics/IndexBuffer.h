#pragma once

#include "Graphics/.Package.h"

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static SPtr<IndexBuffer> Create(float *indices, uint32 size);
};
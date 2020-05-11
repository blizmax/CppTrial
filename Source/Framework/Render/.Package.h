#pragma once

#include "Core/String.h"
#include "Core/Logger.h"
#include "Core/Exception.h"
#include "RenderCore/.Package.h"

class Color;

enum class VertexDataType
{
    Float1,
    Float2,
    Float3,
    Float4,
    Int1,
    Int2,
    Int3,
    Int4,
    UByte4,
};

enum class PixelFormat
{
    R8,
    RGB888,
    RGBA8888,
};

namespace Render
{

uint32 GetVertexDataSize(VertexDataType dataType);
uint32 GetVertexDataComponentCount(VertexDataType dataType);

uint32 GetPixelSize(PixelFormat format);
void PackColor(const Color &color, PixelFormat format, uint8 *dst, uint32 count = 1);
void UnpackColor(Color *dst, PixelFormat format, const uint8 *src, uint32 count = 1);

}
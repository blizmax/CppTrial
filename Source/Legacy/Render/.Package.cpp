#include "Render/.Package.h"

namespace Render
{
uint32 GetVertexDataSize(VertexDataType dataType)
{
    switch (dataType)
    {
    case VertexDataType::Float1:
        return 4;
    case VertexDataType::Float2:
        return 4 * 2;
    case VertexDataType::Float3:
        return 4 * 3;
    case VertexDataType::Float4:
        return 4 * 4;
    case VertexDataType::Int1:
        return 4;
    case VertexDataType::Int2:
        return 4 * 2;
    case VertexDataType::Int3:
        return 4 * 3;
    case VertexDataType::Int4:
        return 4 * 4;
    case VertexDataType::UByte4:
        return 4;
    }
    CT_EXCEPTION(Render, "Unknown data type!");
    return 0;
}

uint32 GetVertexDataComponentCount(VertexDataType dataType)
{
    switch (dataType)
    {
    case VertexDataType::Float1:
        return 1;
    case VertexDataType::Float2:
        return 2;
    case VertexDataType::Float3:
        return 3;
    case VertexDataType::Float4:
        return 4;
    case VertexDataType::Int1:
        return 1;
    case VertexDataType::Int2:
        return 2;
    case VertexDataType::Int3:
        return 3;
    case VertexDataType::Int4:
        return 4;
    case VertexDataType::UByte4:
        return 4;
    }
    CT_EXCEPTION(Render, "Unknown data type!");
    return 0;
}

uint32 GetPixelSize(PixelFormat format)
{
    switch (format)
    {
    case PixelFormat::R8:
        return 1;
    case PixelFormat::RGB888:
        return 4;
    case PixelFormat::RGBA8888:
        return 4;
    }
    CT_EXCEPTION(Render, "Unsupported pixel format!");
    return 0;
}

void PackColor(const Color &color, PixelFormat format, uint8 *dst, uint32 count)
{
    uint32 packed = 0;
    switch (format)
    {
    case PixelFormat::R8:
        Memory::Fill(dst, count, static_cast<uint8>(color.a * 255));
        break;
    case PixelFormat::RGB888:
    case PixelFormat::RGBA8888:
        uint32 value = 0;
        uint32 r = static_cast<uint8>(color.r * 255);
        uint32 g = static_cast<uint8>(color.g * 255);
        uint32 b = static_cast<uint8>(color.b * 255);
        uint32 a = static_cast<uint8>(color.a * 255);
        value = (r << 0) | (g << 8) | (b << 16) | (a << 24);
        Memory::Fill(reinterpret_cast<uint32 *>(dst), count, value);
        break;
    }
}

void UnpackColor(Color *dst, PixelFormat format, const uint8 *src, uint32 count)
{
    Color color = Color::BLACK;
    switch (format)
    {
    case PixelFormat::R8:
        color.a = *src / 255.0f;
        break;
    case PixelFormat::RGB888:
    case PixelFormat::RGBA8888:
        uint32 value = *reinterpret_cast<const uint32 *>(src);
        uint32 a = value >> 24;
        uint32 b = (value & 0x00ff0000) >> 16;
        uint32 g = (value & 0x0000ff00) >> 8;
        uint32 r = (value & 0x000000ff);
        color = Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
        break;
    }
    Memory::Fill(dst, count, color);
}
}
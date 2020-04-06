#include "RenderOpenGL/.Package.h"

namespace Render
{
uint32 GetGLDataType(VertexDataType dataType)
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

uint32 GetGLPixelFormat(PixelFormat format)
{
    switch (format)
    {
    case PixelFormat::R8:
        return GL_RED;
    case PixelFormat::RGB888:
        return GL_RGB;
    case PixelFormat::RGBA8888:
        return GL_RGBA;
    }

    CT_EXCEPTION(Render, "Unsupported pixel format!");
    return 0;
}

uint32 GetGLPixelDataType(PixelFormat format)
{
    switch (format)
    {
    case PixelFormat::R8:
    case PixelFormat::RGB888:
    case PixelFormat::RGBA8888:
        return GL_UNSIGNED_BYTE;
    }

    CT_EXCEPTION(Render, "Unsupported pixel format!");
    return 0;
}

uint32 GetGLPixelInternalFormat(PixelFormat format)
{
    switch (format)
    {
    case PixelFormat::R8:
        return GL_R8;
    case PixelFormat::RGB888:
        return GL_RGB8;
    case PixelFormat::RGBA8888:
        return GL_RGBA8;
    }

    CT_EXCEPTION(Render, "Unsupported pixel format!");
    return 0;
}

uint32 GetGLTextureWrap(TextureWrap wrap)
{
    switch (wrap)
    {
    case TextureWrap::Repeat:
        return GL_REPEAT;
    case TextureWrap::Mirror:
        return GL_MIRRORED_REPEAT;
    case TextureWrap::Clamp:
        return GL_CLAMP_TO_EDGE;
    case TextureWrap::Border:
        return GL_CLAMP_TO_BORDER;
    }

    CT_EXCEPTION(Render, "Unsupported texture wrap!");
    return 0;
}

uint32 GetGLTextureFilter(TextureFilter filter)
{
    switch (filter)
    {
    case TextureFilter::Nearest:
        return GL_NEAREST;
    case TextureFilter::Linear:
        return GL_LINEAR;
    }

    CT_EXCEPTION(Render, "Unsupported texture filter!");
    return 0;
}

uint32 GetGLMinMipFilter(TextureFilter min, TextureFilter mip)
{
    switch (min)
    {
    case TextureFilter::Nearest:
        switch (mip)
        {
        case TextureFilter::Nearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        case TextureFilter::Linear:
            return GL_NEAREST_MIPMAP_LINEAR;
        }
        break;
    case TextureFilter::Linear:
        switch (mip)
        {
        case TextureFilter::Nearest:
            return GL_LINEAR_MIPMAP_NEAREST;
        case TextureFilter::Linear:
            return GL_LINEAR_MIPMAP_LINEAR;
        }
        break;
    }

    CT_EXCEPTION(Render, "Unsupported texture filter!");
    return 0;
}

uint32 GetGLUsage(GpuBufferUsage usage)
{
    switch (usage)
    {
    case GpuBufferUsage::Static:
        return GL_STATIC_DRAW;
    case GpuBufferUsage::Dynamic:
        return GL_DYNAMIC_DRAW;
    }

    CT_EXCEPTION(Render, "Unsupported usage.");
    return 0;
}

uint32 GetGLBlendFactor(BlendFactor factor)
{
    switch (factor)
    {
    case BlendFactor::Zero:
        return GL_ZERO;
    case BlendFactor::One:
        return GL_ONE;
    case BlendFactor::SrcColor:
        return GL_SRC_COLOR;
    case BlendFactor::OneMinusSrcColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case BlendFactor::DstColor:
        return GL_DST_COLOR;
    case BlendFactor::OneMinusDstColor:
        return GL_ONE_MINUS_DST_COLOR;
    case BlendFactor::SrcAlpha:
        return GL_SRC_ALPHA;
    case BlendFactor::OneMinusSrcAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DstAlpha:
        return GL_DST_ALPHA;
    case BlendFactor::OneMinusDstAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    }

    CT_EXCEPTION(Render, "Unsupported blend factor.");
    return 0;
}

uint32 GetGLBlendOperation(BlendOperation operation)
{
    switch (operation)
    {
    case BlendOperation::Add:
        return GL_FUNC_ADD;
    case BlendOperation::Sub:
        return GL_FUNC_SUBTRACT;
    case BlendOperation::ReverseSub:
        return GL_FUNC_REVERSE_SUBTRACT;
    }

    CT_EXCEPTION(Render, "Unsupported blend operation.");
    return 0;
}

uint32 GetGLStencilOperation(StencilOperation operation)
{
    switch (operation)
    {
    case StencilOperation::Zero:
        return GL_ZERO;
    case StencilOperation::Keep:
        return GL_KEEP;
    case StencilOperation::Replace:
        return GL_REPLACE;
    case StencilOperation::Increment:
        return GL_INCR;
    case StencilOperation::Decrement:
        return GL_DECR;
    case StencilOperation::IncrementWrap:
        return GL_INCR_WRAP;
    case StencilOperation::DecrementWrap:
        return GL_DECR_WRAP;
    case StencilOperation::Invert:
        return GL_INVERT;
    }

    CT_EXCEPTION(Render, "Unsupported stencil operation.");
    return 0;
}

uint32 GetCompareOperation(CompareOperation operation)
{
    switch (operation)
    {
    case CompareOperation::AlwaysFail:
        return GL_NEVER;
    case CompareOperation::AlwaysPass:
        return GL_ALWAYS;
    case CompareOperation::Less:
        return GL_LESS;
    case CompareOperation::LessEqual:
        return GL_LEQUAL;
    case CompareOperation::Equal:
        return GL_EQUAL;
    case CompareOperation::NotEqual:
        return GL_NOTEQUAL;
    case CompareOperation::Greater:
        return GL_GREATER;
    case CompareOperation::GreaterEqual:
        return GL_GEQUAL;
    }

    CT_EXCEPTION(Render, "Unsupported compare operation.");
    return 0;
}
}
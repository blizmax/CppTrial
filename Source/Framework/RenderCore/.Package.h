#pragma once

#include "Core/String.h"
#include "Core/Logger.h"
#include "Core/Exception.h"

enum class VertexDataType
{
    Unknown,
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

enum class TextureFilter
{
    Nearest,
    Linear,
    //Anisotropic,
};

enum class TextureWrap
{
    Repeat,
    Mirror,
    Clamp,
    Border,
};

enum class GpuBufferUsage
{
    Static,
    Dynamic,
};

enum class GpuQueueType
{
    Graphics,
};

enum class BlendFactor
{
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,
};

enum class BlendOperation
{
    Add,
    Sub,        //src - dst
    ReverseSub, //dst - src
};

enum class CompareOperation
{
    AlwaysFail,
    AlwaysPass,
    Less,
    LessEqual,
    Equal,
    NotEqual,
    Greater,
    GreaterEqual,
};

enum class StencilOperation
{
    Zero,
    Keep,
    Replace,
    Increment,
    Decrement,
    IncrementWrap,
    DecrementWrap,
    Invert,
};

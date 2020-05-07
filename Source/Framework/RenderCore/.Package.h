#pragma once

#include "Core/String.h"
#include "Core/Logger.h"
#include "Core/Exception.h"

enum class ResourceType
{
    Buffer,
    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
};

enum class ResourceState
{
    Undefined,
    PreInitialized,

    Present,
};


enum class ShaderType
{
    Vertex,
    Fragment,
    TessControl,
    TessEvaluation,
    Geometry,
    Compute,

    Count,
};

enum class ShaderVisibility
{
    None = 0,
    Vertex = (1 << (int32)ShaderType::Vertex),
    Fragment = (1 << (int32)ShaderType::Fragment),
    TessControl = (1 << (int32)ShaderType::TessControl),
    TessEvaluation = (1 << (int32)ShaderType::TessEvaluation),
    Geometry = (1 << (int32)ShaderType::Geometry),
    Compute = (1 << (int32)ShaderType::Compute),

    All = (1 << (int32)ShaderType::Count) - 1,
};

enum class Topology
{
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip,
};

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

enum class DescriptorType
{
    Sampler,
};

enum class PixelFormat
{
    Unknown,
    R8,
    RGB888,
    RGBA8888,
};

enum class TextureFilter
{
    Nearest,
    Linear,
};

enum class TextureWrap
{
    Repeat,
    Mirror,
    Clamp,
    Border,
};

enum class PolygonMode
{
    Fill,
    Wireframe,
};

enum class CullMode
{
    None,
    Front,
    Back,
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

namespace RenderCore
{
    constexpr int32 COLOR_ATTCHMENT_MAX_NUM = 4;
    constexpr int32 VERTEX_INPUT_MAX_NUM = 16;

    uint32 GetVertexDataSize(VertexDataType dataType);
    uint32 GetVertexDataComponentCount(VertexDataType dataType);
}
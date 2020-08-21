#pragma once

#include "Core/Exception.h"
#include "Core/Flags.h"
#include "Core/Logger.h"
#include "Core/String.h"
#include "Math/Color.h"
#include "Math/IntVector.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

enum class ResourceType
{
    Buffer,
    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
    Texture2DMultisample,
};

enum class ResourceState
{
    Undefined,
    PreInitialized,
    Common,
    VertexBuffer,
    ConstantBuffer,
    IndexBuffer,
    RenderTarget,
    UnorderedAccess,
    DepthStencil,
    ShaderResource,
    StreamOut,
    IndirectArg,
    CopyDest,
    CopySource,
    ResolveDest,
    ResolveSource,
    Present,
    GenericRead,
    Predication,
    PixelShader,
    NonPixelShader,
};

CT_DECL_FLAGS(ResourceBind){
    None = 0,
    Vertex = 1 << 0,
    Index = 1 << 1,
    Constant = 1 << 2,
    StreamOutput = 1 << 3,
    ShaderResource = 1 << 4,
    UnorderedAccess = 1 << 5,
    RenderTarget = 1 << 6,
    DepthStencil = 1 << 7,
    IndirectArg = 1 << 8,
    Shared = 1 << 9,
};

enum class BufferCpuAccess
{
    None,
    Write,
    Read,
};

enum class BufferMapType
{
    Read,
    Write,
    WriteDiscard,
};

enum class MemoryUsage
{
    Default,
    Upload,
    Download,
};

enum class ShaderType
{
    Vertex,
    Pixel,
    Hull,
    Domain,
    Geometry,
    Compute,

    Count,
};

enum class ShaderLanguage
{
    Unknown,
    Glsl,
    Hlsl,
};

CT_DECL_FLAGS(ShaderVisibility){
    None = 0,
    Vertex = (1 << (int32)ShaderType::Vertex),
    Pixel = (1 << (int32)ShaderType::Pixel),
    Hull = (1 << (int32)ShaderType::Hull),
    Domain = (1 << (int32)ShaderType::Domain),
    Geometry = (1 << (int32)ShaderType::Geometry),
    Compute = (1 << (int32)ShaderType::Compute),

    All = (1 << (int32)ShaderType::Count) - 1,
};

enum class Topology
{
    Undefined,
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip,
};

enum class DescriptorType
{
    Unknown = -1,

    Sampler,
    TextureSrv,
    TextureUav,
    RawBufferSrv,
    RawBufferUav,
    TypedBufferSrv,
    TypedBufferUav,
    StructuredBufferSrv,
    StructuredBufferUav,
    Cbv,
    Dsv,
    Rtv,

    Count,
};

enum class QueryType
{
    Timestamp,
    Occlusion,
    PipelineStats,
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
    Transfer,
    Compute,
    Graphics,

    Count,
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

enum class ResourceFormat
{
    Unknown,

    R8Unorm,
    R8Snorm,
    R16Unorm,
    R16Snorm,
    RG8Unorm,
    RG8Snorm,
    RG16Unorm,
    RG16Snorm,
    RGB16Unorm,
    RGB16Snorm,
    R24UnormX8,
    RGB5A1Unorm,
    RGBA8Unorm,
    RGBA8Snorm,
    RGB10A2Unorm,
    RGB10A2UInt,
    RGBA16Unorm,
    RGBA8UnormSrgb,
    R16Float,
    RG16Float,
    RGB16Float,
    RGBA16Float,
    R32Float,
    R32FloatX32,
    RG32Float,
    RGB32Float,
    RGBA32Float,
    R11G11B10Float,
    RGB9E5Float,
    R8Int,
    R8UInt,
    R16Int,
    R16UInt,
    R32Int,
    R32UInt,
    RG8Int,
    RG8UInt,
    RG16Int,
    RG16UInt,
    RG32Int,
    RG32UInt,
    RGB16Int,
    RGB16UInt,
    RGB32Int,
    RGB32UInt,
    RGBA8Int,
    RGBA8UInt,
    RGBA16Int,
    RGBA16UInt,
    RGBA32Int,
    RGBA32UInt,

    BGRA8Unorm,
    BGRA8UnormSrgb,

    BGRX8Unorm,
    BGRX8UnormSrgb,
    Alpha8Unorm,
    Alpha32Float,
    R5G6B5Unorm,

    // Depth-stencil
    D32Float,
    D16Unorm,
    D32FloatS8X24,
    D24UnormS8,

    // Compressed
    BC1Unorm,
    BC1UnormSrgb,
    BC2Unorm,
    BC2UnormSrgb,
    BC3Unorm,
    BC3UnormSrgb,
    BC4Unorm,
    BC4Snorm,
    BC5Unorm,
    BC5Snorm,
    BC6HS16,
    BC6HU16,
    BC7Unorm,
    BC7UnormSrgb,

    Count,
};

//Start from left top.
struct Viewport
{
    float x = 0.0f;
    float y = 0.0f;
    float width = 1.0f;
    float height = 1.0f;
    float minDepth = 0.0f; //range: 0-1
    float maxDepth = 1.0f; //range: 0-1
};

struct Scissor
{
    int32 x;
    int32 y;
    uint32 width;
    uint32 height;
};

struct DrawIndirectArgs
{
    uint32 vertexCount;
    uint32 instanceCount;
    uint32 firstVertex;
    uint32 firstInstance;
};

struct DrawIndexedIndirectArgs
{
    uint32 indexCount;
    uint32 instanceCount;
    uint32 firstIndex;
    int32 vertexOffset;
    uint32 firstInstance;
};

CT_DECL_FLAGS(GraphicsStateBind){
    None = 0,
    Vars = 1 << 0,
    PrimitiveTopology = 1 << 1,
    Vao = 1 << 2,
    Fbo = 1 << 3,
    Viewports = 1 << 4,
    Scissors = 1 << 5,
    PipelineState = 1 << 6,
    SamplePositions = 1 << 7,

    All = (uint32)-1,
};

enum class ResourceComponentType
{
    Unknown,

    Float,
    Unorm,
    Snorm,
    UnormSrgb,
    UInt,
    Int,
};

struct ResourceFormatDesc
{
    ResourceFormat format;
    const char8 *name;
    uint32 bytes;
    uint32 componentCount;
    ResourceComponentType componentType;
    struct
    {
        bool isDepth;
        bool isStencil;
        bool isCompressed;
    };
    struct
    {
        uint32 width;
        uint32 height;
    } compressionRatio;
    uint32 componentBits[4];
};

class Resource;
class Buffer;
class Texture;
class ResourceView;
class Sampler;
class RootSignature;
class Program;
class CopyContext;
class ComputeContext;
class RenderContext;
class ShaderVar;

constexpr int32 COLOR_ATTCHMENT_MAX_NUM = 8;
constexpr int32 VERTEX_INPUT_MAX_NUM = 16;
constexpr int32 VIEWPORT_MAX_NUM = 16;

const ResourceFormatDesc &GetResourceFormatDesc(ResourceFormat format);
ResourceComponentType GetResourceComponentType(ResourceFormat format);
uint32 GetResourceFormatBytes(ResourceFormat format);
uint32 GetResourceFormatComponentCount(ResourceFormat format);
uint32 GetResourceFormatWidthCompressionRatio(ResourceFormat format);
uint32 GetResourceFormatHeightCompressionRatio(ResourceFormat format);
bool IsCompressedFormat(ResourceFormat format);
bool IsDepthFormat(ResourceFormat format);
bool IsStencilFormat(ResourceFormat format);
bool IsDepthStencilFormat(ResourceFormat format);
bool IsSrgbFormat(ResourceFormat format);
ResourceFormat LinearToSrgbFormat(ResourceFormat format);
ResourceFormat SrgbToLinearFormat(ResourceFormat format);

/** API dependent */
ResourceBindFlags GetResourceFormatBindFlags(ResourceFormat format);
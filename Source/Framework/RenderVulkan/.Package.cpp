#include "RenderVulkan/.Package.h"

namespace RenderCore
{
VkAllocationCallbacks *gVulkanAlloc = nullptr;

VkImageLayout ToVkImageLayout(ResourceState state)
{
    switch (state)
    {
    case ResourceState::Undefined:
        return VK_IMAGE_LAYOUT_UNDEFINED;
    case ResourceState::PreInitialized:
        return VK_IMAGE_LAYOUT_PREINITIALIZED;

    case ResourceState::Present:
        return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }

    CT_EXCEPTION(RenderCore, "Unsupported resource state!");
    return VK_IMAGE_LAYOUT_UNDEFINED;
}

VkShaderStageFlags ToVkShaderVisibility(ShaderVisibilityFlags visibility)
{
    VkShaderStageFlags flags = 0;

    if (visibility & ShaderVisibility::Vertex)
        flags |= VK_SHADER_STAGE_VERTEX_BIT;
    if (visibility & ShaderVisibility::Fragment)
        flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
    if (visibility & ShaderVisibility::TessControl)
        flags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;;
    if (visibility & ShaderVisibility::TessEvaluation)
        flags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    if (visibility & ShaderVisibility::Geometry)
        flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
    if (visibility & ShaderVisibility::Compute)
        flags |= VK_SHADER_STAGE_COMPUTE_BIT;

    return flags;
}

VkDescriptorType ToVkDescriptorType(DescriptorType descType)
{
    switch (descType)
    {
    case DescriptorType::Sampler:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    case DescriptorType::TextureSrv:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    case DescriptorType::TextureUav:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    case DescriptorType::RawBufferSrv:
    case DescriptorType::TypedBufferSrv:
        return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
    case DescriptorType::RawBufferUav:
    case DescriptorType::TypedBufferUav:
        return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
    case DescriptorType::Cbv:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case DescriptorType::StructuredBufferSrv:
    case DescriptorType::StructuredBufferUav:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case DescriptorType::Dsv:
    case DescriptorType::Rtv:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    }

    CT_EXCEPTION(RenderCore, "Unsupported desc type!");
    return VK_DESCRIPTOR_TYPE_MAX_ENUM;
}

VkBufferUsageFlags ToVkBufferUsage(ResourceBindFlags bind)
{
    VkBufferUsageFlags flags = 0;
    if (bind & ResourceBind::Vertex)
        flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if (bind & ResourceBind::Index)
        flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    if (bind & ResourceBind::UnorderedAccess)
        flags |= (VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    if (bind & ResourceBind::ShaderResource)
        flags |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
    if (bind & ResourceBind::IndirectArg)
        flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    if (bind & ResourceBind::Constant)
        flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    return flags;
}

VkMemoryPropertyFlags ToVkMemoryProperty(MemoryUsage usage)
{
    switch (usage)
    {
    case MemoryUsage::Default:
        return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    case MemoryUsage::Upload:
        return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    case MemoryUsage::Download:
        return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
    }
    return 0;
}

VkFormat ToVkDataFormat(VertexDataType dataType)
{
    switch (dataType)
    {
    case VertexDataType::Float1:
        return VK_FORMAT_R32_SFLOAT;
    case VertexDataType::Float2:
        return VK_FORMAT_R32G32_SFLOAT;
    case VertexDataType::Float3:
        return VK_FORMAT_R32G32B32_SFLOAT;
    case VertexDataType::Float4:
        return VK_FORMAT_R32G32B32A32_SFLOAT;
    case VertexDataType::Int1:
        return VK_FORMAT_R32_SINT;
    case VertexDataType::Int2:
        return VK_FORMAT_R32G32_SINT;
    case VertexDataType::Int3:
        return VK_FORMAT_R32G32B32_SINT;
    case VertexDataType::Int4:
        return VK_FORMAT_R32G32B32A32_SINT;
    case VertexDataType::UByte4:
        return VK_FORMAT_R8G8B8A8_UINT;
    }

    CT_EXCEPTION(RenderCore, "Unknown data type!");
    return VK_FORMAT_UNDEFINED;
}

VkPolygonMode ToVkPolygonMode(PolygonMode mode)
{
    switch (mode)
    {
    case PolygonMode::Fill:
        return VK_POLYGON_MODE_FILL;
    case PolygonMode::Wireframe:
        return VK_POLYGON_MODE_LINE;
    }

    CT_EXCEPTION(RenderCore, "Unsupported polygon mode.");
    return VK_POLYGON_MODE_FILL;
}

VkCullModeFlagBits ToVkCullMode(CullMode mode)
{
    switch (mode)
    {
    case CullMode::None:
        return VK_CULL_MODE_NONE;
    case CullMode::Front:
        return VK_CULL_MODE_FRONT_BIT;
    case CullMode::Back:
        return VK_CULL_MODE_BACK_BIT;
    }

    CT_EXCEPTION(RenderCore, "Unsupported cull mode.");
    return VK_CULL_MODE_NONE;
}

VkBlendFactor ToVkBlendFactor(BlendFactor factor)
{
    switch (factor)
    {
    case BlendFactor::Zero:
        return VK_BLEND_FACTOR_ZERO;
    case BlendFactor::One:
        return VK_BLEND_FACTOR_ONE;
    case BlendFactor::SrcColor:
        return VK_BLEND_FACTOR_SRC_COLOR;
    case BlendFactor::OneMinusSrcColor:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case BlendFactor::DstColor:
        return VK_BLEND_FACTOR_DST_COLOR;
    case BlendFactor::OneMinusDstColor:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case BlendFactor::SrcAlpha:
        return VK_BLEND_FACTOR_SRC_ALPHA;
    case BlendFactor::OneMinusSrcAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DstAlpha:
        return VK_BLEND_FACTOR_DST_ALPHA;
    case BlendFactor::OneMinusDstAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    }

    CT_EXCEPTION(RenderCore, "Unsupported blend factor.");
    return VK_BLEND_FACTOR_ZERO;
}

VkBlendOp ToVkBlendOperation(BlendOperation operation)
{
    switch (operation)
    {
    case BlendOperation::Add:
        return VK_BLEND_OP_ADD;
    case BlendOperation::Sub:
        return VK_BLEND_OP_SUBTRACT;
    case BlendOperation::ReverseSub:
        return VK_BLEND_OP_REVERSE_SUBTRACT;
    }

    CT_EXCEPTION(RenderCore, "Unsupported blend operation.");
    return VK_BLEND_OP_ADD;
}

VkStencilOp ToVkStencilOperation(StencilOperation operation)
{
    switch (operation)
    {
    case StencilOperation::Zero:
        return VK_STENCIL_OP_ZERO;
    case StencilOperation::Keep:
        return VK_STENCIL_OP_KEEP;
    case StencilOperation::Replace:
        return VK_STENCIL_OP_REPLACE;
    case StencilOperation::Increment:
        return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
    case StencilOperation::Decrement:
        return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
    case StencilOperation::IncrementWrap:
        return VK_STENCIL_OP_INCREMENT_AND_WRAP;
    case StencilOperation::DecrementWrap:
        return VK_STENCIL_OP_DECREMENT_AND_WRAP;
    case StencilOperation::Invert:
        return VK_STENCIL_OP_INVERT;
    }

    CT_EXCEPTION(RenderCore, "Unsupported stencil operation.");
    return VK_STENCIL_OP_ZERO;
}

VkCompareOp ToVkCompareOperation(CompareOperation operation)
{
    switch (operation)
    {
    case CompareOperation::AlwaysFail:
        return VK_COMPARE_OP_NEVER;
    case CompareOperation::AlwaysPass:
        return VK_COMPARE_OP_ALWAYS;
    case CompareOperation::Less:
        return VK_COMPARE_OP_LESS;
    case CompareOperation::LessEqual:
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    case CompareOperation::Equal:
        return VK_COMPARE_OP_EQUAL;
    case CompareOperation::NotEqual:
        return VK_COMPARE_OP_NOT_EQUAL;
    case CompareOperation::Greater:
        return VK_COMPARE_OP_GREATER;
    case CompareOperation::GreaterEqual:
        return VK_COMPARE_OP_GREATER_OR_EQUAL;
    }

    CT_EXCEPTION(RenderCore, "Unsupported compare operation.");
    return VK_COMPARE_OP_NEVER;
}

VkSampleCountFlagBits ToVkSampleCount(int32 sample)
{
    switch (sample)
    {
    case 0:
    case 1:
        return VK_SAMPLE_COUNT_1_BIT;
    case 2:
        return VK_SAMPLE_COUNT_2_BIT;
    case 4:
        return VK_SAMPLE_COUNT_4_BIT;
    case 8:
        return VK_SAMPLE_COUNT_8_BIT;
    case 16:
        return VK_SAMPLE_COUNT_16_BIT;
    case 32:
        return VK_SAMPLE_COUNT_32_BIT;
    case 64:
        return VK_SAMPLE_COUNT_64_BIT;
    }

    CT_EXCEPTION(RenderCore, "Unsupported sample count.");
    return VK_SAMPLE_COUNT_1_BIT;
}

VkSamplerAddressMode ToVkAddressMode(TextureWrap wrap)
{
    switch (wrap)
    {
    case TextureWrap::Repeat:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case TextureWrap::Mirror:
        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case TextureWrap::Clamp:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case TextureWrap::Border:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    }

    CT_EXCEPTION(RenderCore, "Unsupported texture wrap mode.");
    return VK_SAMPLER_ADDRESS_MODE_REPEAT;
}

}
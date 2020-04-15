#include "RenderVulkan/.Package.h"

namespace RenderCore
{
    VkAllocationCallbacks *gVulkanAlloc = nullptr;

    VkPolygonMode ToVkPolygonMode(PolygonMode mode)
    {
        switch (mode)
		{
		case PolygonMode::Fill:
            return VK_POLYGON_MODE_FILL;
		case PolygonMode::Wireframe:
			return VK_POLYGON_MODE_LINE;
		}

        CT_EXCEPTION(Render, "Unsupported polygon mode.");
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

        CT_EXCEPTION(Render, "Unsupported cull mode.");
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

        CT_EXCEPTION(Render, "Unsupported blend factor.");
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

        CT_EXCEPTION(Render, "Unsupported blend operation.");
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

        CT_EXCEPTION(Render, "Unsupported stencil operation.");
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

        CT_EXCEPTION(Render, "Unsupported compare operation.");
        return VK_COMPARE_OP_NEVER;
    }

}
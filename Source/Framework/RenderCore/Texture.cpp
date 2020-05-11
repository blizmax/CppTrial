#include "RenderCore/Texture.h"
#include "Core/Math.h"

namespace RenderCore
{

Texture::Texture(uint32 width, uint32 height, uint32 depth, uint32 arrayLayers, uint32 mipLevels, uint32 sampleCount, ResourceFormat format, ResourceType resourceType, ResourceBindFlags flags, const void *data)
    : Resource(resourceType, flags, 0), width(width), height(height), depth(depth), arrayLayers(arrayLayers), mipLevels(mipLevels), sampleCount(sampleCount), format(format)
{
    CT_CHECK(width > 0 && height > 0 && depth > 0);
    CT_CHECK(arrayLayers > 0 && mipLevels > 0 && sampleCount > 0);

    //TODO
    //Check mipLevels
}

uint32 GetMaxMipLevel(uint32 width, uint32 height, uint32 depth)
{
    auto dim = Math::Max(width, height, depth);
    return 1 + (uint32)Math::Log2((float)dim);
}
}
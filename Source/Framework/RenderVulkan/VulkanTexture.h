#pragma once

#include "RenderVulkan/VulkanImage.h"
#include "RenderCore/Texture.h"

namespace RenderCore
{
class VulkanTexture : public Texture
{
public:

private:
    uint32 width = 1;
    uint32 height = 1;
    uint32 depth = 1;
    uint32 mipLevels = 0;
    PixelFormat format;
};
}
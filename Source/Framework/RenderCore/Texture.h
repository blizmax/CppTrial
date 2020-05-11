#pragma once

#include "RenderCore/Resource.h"

namespace RenderCore
{

class Texture : public Resource
{
public:
    virtual ~Texture() = default;

    uint32 GetWidth(uint32 mipLevel = 0) const
    {
        if(mipLevel == 0 || mipLevel < mipLevels)
            return width >> mipLevel;
        return 0; 
    }

    uint32 GetHeight(uint32 mipLevel = 0) const
    {
        if (mipLevel == 0 || mipLevel < mipLevels)
            return height >> mipLevel;
        return 0;
    }

    uint32 GetDepth(uint32 mipLevel = 0) const
    {
        if (mipLevel == 0 || mipLevel < mipLevels)
            return depth >> mipLevel;
        return 0;
    }

    uint32 GetMipLevels() const
    {
        return mipLevels;
    }

    uint32 GetArrayLayers() const
    {
        return arrayLayers;
    }

    uint32 GetSampleCount() const
    {
        return sampleCount;
    }

    ResourceFormat GetFormat() const
    {
        return format;
    }

    static SPtr<Texture> Create2D(uint32 width, uint32 height, ResourceFormat format, uint32 arrayLayers = 1, uint32 mipLevels = UINT32_MAX, const void* data = nullptr);

protected:
    uint32 width = 1;
    uint32 height = 1;
    uint32 depth = 1;
    uint32 mipLevels = 0;
    uint32 arrayLayers = 1;
    uint32 sampleCount = 1;
    ResourceFormat format = ResourceFormat::Unknown;
};

}
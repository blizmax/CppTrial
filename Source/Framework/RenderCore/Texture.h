#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

class Texture
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

    PixelFormat GetFormat() const
    {
        return format;
    }

    static SPtr<Texture> Create();

protected:
    uint32 width = 1;
    uint32 height = 1;
    uint32 depth = 1;
    uint32 mipLevels = 0;
    uint32 arrayLayers = 1;
    uint32 sampleCount = 1;
    PixelFormat format = PixelFormat::RGBA8888;
};

}
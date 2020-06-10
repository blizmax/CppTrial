#pragma once

#include "RenderCore/Resource.h"

namespace RenderCore
{

class RenderContext;

class Texture : public Resource
{
public:
    Texture(uint32 width, uint32 height, uint32 depth, uint32 arrayLayers, uint32 mipLevels, uint32 sampleCount, ResourceFormat format, ResourceType resourceType, ResourceBindFlags flags);
    virtual ~Texture() = default;

    virtual ResourceState GetSubresourceState(uint32 arraySlice, uint32 mipLevel) const override
    {
        uint32 subresouce = GetSubresourceIndex(arraySlice, mipLevel);
        return stateData.global ? stateData.state : stateData.subStates[subresouce];
    }

    virtual void SetSubresourceState(uint32 arraySlice, uint32 mipLevel, ResourceState newState) const override
    {
        if(IsStateGlobal())
        {
            for(auto &e : stateData.subStates)
            {
                e = stateData.state;
            }
        }
        stateData.global = false;
        uint32 subresouce = GetSubresourceIndex(arraySlice, mipLevel);
        stateData.subStates[subresouce] = newState;
    }

    virtual SPtr<ResourceView> GetSrv() override
    {
        return GetSrv(0);
    }

    virtual SPtr<ResourceView> GetUav() override
    {
        return GetUav(0);
    }

    virtual SPtr<Texture> AsTexture() override
    {
        return weakThis.lock();
    }

    void ClearViews();
    void GenerateMips(RenderContext *ctx);

    SPtr<ResourceView> GetSrv(uint32 mostDetailedMip, uint32 mipLevels = UINT32_MAX, uint32 firstArraySlice = 0, uint32 arrayLayers = UINT32_MAX);
    SPtr<ResourceView> GetUav(uint32 mipLevel, uint32 firstArraySlice = 0, uint32 arrayLayers = UINT32_MAX);
    SPtr<ResourceView> GetRtv(uint32 mipLevel = 0, uint32 firstArraySlice = 0, uint32 arrayLayers = UINT32_MAX);
    SPtr<ResourceView> GetDsv(uint32 mipLevel = 0, uint32 firstArraySlice = 0, uint32 arrayLayers = UINT32_MAX);

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

    ResourceFormat GetResourceFormat() const
    {
        return format;
    }

    uint32 GetSubresourceArraySlice(uint32 subresource) const
    {
        return subresource / mipLevels;
    }

    uint32 GetSubresourceMipLevel(uint32 subresource) const
    {
        return subresource % mipLevels;
    }

    uint32 GetSubresourceIndex(uint32 arraySlice, uint32 mipLevel) const
    {
        return mipLevel + arraySlice * mipLevels;
    }

    static SPtr<Texture> Create(uint32 width, uint32 height, uint32 depth, ResourceFormat format, ResourceType resourceType, uint32 arrayLayers = 1, uint32 mipLevels = UINT32_MAX, uint32 sampleCount = 1, const void *data = nullptr, ResourceBindFlags flags = ResourceBind::ShaderResource);
    static SPtr<Texture> Create1D(uint32 width, ResourceFormat format, uint32 arrayLayers = 1, uint32 mipLevels = UINT32_MAX, const void *data = nullptr, ResourceBindFlags flags = ResourceBind::ShaderResource);
    static SPtr<Texture> Create2D(uint32 width, uint32 height, ResourceFormat format, uint32 arrayLayers = 1, uint32 mipLevels = UINT32_MAX, const void *data = nullptr, ResourceBindFlags flags = ResourceBind::ShaderResource);
    static SPtr<Texture> Create3D(uint32 width, uint32 height, uint32 depth, ResourceFormat format, uint32 arrayLayers = 1, uint32 mipLevels = UINT32_MAX, const void *data = nullptr, ResourceBindFlags flags = ResourceBind::ShaderResource);
    static SPtr<Texture> CreateCube(uint32 width, uint32 height, ResourceFormat format, uint32 arrayLayers = 1, uint32 mipLevels = UINT32_MAX, const void *data = nullptr, ResourceBindFlags flags = ResourceBind::ShaderResource);
    static SPtr<Texture> Create2DMS(uint32 width, uint32 height, ResourceFormat format, uint32 sampleCount, uint32 arrayLayers = 1, ResourceBindFlags flags = ResourceBind::ShaderResource);

    static SPtr<Texture> Create2D(uint32 width, uint32 height, ResourceFormat format, void *handle, ResourceBindFlags flags = ResourceBind::ShaderResource);

protected:
    virtual void InitData(const void *data, bool autoGenMips) = 0;

    uint32 GetMaxMipLevel(uint32 width, uint32 height, uint32 depth) const;
    void CheckViewParams(uint32 &mostDetailedMip, uint32 &mipLevels, uint32 &firstArraySlice, uint32 &arrayLayers) const;

protected: 
    uint32 width = 1;
    uint32 height = 1;
    uint32 depth = 1;
    uint32 mipLevels = 1;
    uint32 arrayLayers = 1;
    uint32 sampleCount = 1;
    ResourceFormat format = ResourceFormat::Unknown;

    Array<SPtr<ResourceView>> srvs;
    Array<SPtr<ResourceView>> uavs;
    Array<SPtr<ResourceView>> rtvs;
    Array<SPtr<ResourceView>> dsvs;
    WPtr<Texture> weakThis;
};

}
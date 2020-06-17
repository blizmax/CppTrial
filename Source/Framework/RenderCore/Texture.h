#pragma once

#include "RenderCore/Resource.h"

namespace RenderCore
{

class RenderContext;

class Texture : public Resource
{
public:
    Texture(int32 width, int32 height, int32 depth, int32 arrayLayers, int32 mipLevels, int32 sampleCount, ResourceFormat format, ResourceType resourceType, ResourceBindFlags flags);
    virtual ~Texture() = default;

    virtual ResourceState GetSubresourceState(int32 arraySlice, int32 mipLevel) const override
    {
        auto subresouce = GetSubresourceIndex(arraySlice, mipLevel);
        return stateData.global ? stateData.state : stateData.subStates[subresouce];
    }

    virtual void SetSubresourceState(int32 arraySlice, int32 mipLevel, ResourceState newState) const override
    {
        if(IsStateGlobal())
        {
            for(auto &e : stateData.subStates)
            {
                e = stateData.state;
            }
        }
        stateData.global = false;
        auto subresouce = GetSubresourceIndex(arraySlice, mipLevel);
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

    SPtr<ResourceView> GetSrv(int32 mostDetailedMip, int32 mipLevels = -1, int32 firstArraySlice = 0, int32 arrayLayers = -1);
    SPtr<ResourceView> GetUav(int32 mipLevel, int32 firstArraySlice = 0, int32 arrayLayers = -1);
    SPtr<ResourceView> GetRtv(int32 mipLevel = 0, int32 firstArraySlice = 0, int32 arrayLayers = -1);
    SPtr<ResourceView> GetDsv(int32 mipLevel = 0, int32 firstArraySlice = 0, int32 arrayLayers = -1);

    int32 GetWidth(int32 mipLevel = 0) const
    {
        if(mipLevel == 0 || mipLevel < mipLevels)
            return width >> mipLevel;
        return 0; 
    }

    int32 GetHeight(int32 mipLevel = 0) const
    {
        if (mipLevel == 0 || mipLevel < mipLevels)
            return height >> mipLevel;
        return 0;
    }

    int32 GetDepth(int32 mipLevel = 0) const
    {
        if (mipLevel == 0 || mipLevel < mipLevels)
            return depth >> mipLevel;
        return 0;
    }

    int32 GetMipLevels() const
    {
        return mipLevels;
    }

    int32 GetArrayLayers() const
    {
        return arrayLayers;
    }

    int32 GetSampleCount() const
    {
        return sampleCount;
    }

    ResourceFormat GetResourceFormat() const
    {
        return format;
    }

    int32 GetSubresourceArraySlice(int32 subresource) const
    {
        return subresource / mipLevels;
    }

    int32 GetSubresourceMipLevel(int32 subresource) const
    {
        return subresource % mipLevels;
    }

    int32 GetSubresourceIndex(int32 arraySlice, int32 mipLevel) const
    {
        return mipLevel + arraySlice * mipLevels;
    }

    static SPtr<Texture> Create(int32 width, int32 height, int32 depth, ResourceFormat format, ResourceType resourceType, int32 arrayLayers = 1, int32 mipLevels = -1, int32 sampleCount = 1, const void *data = nullptr, ResourceBindFlags flags = ResourceBind::ShaderResource);
    static SPtr<Texture> Create1D(int32 width, ResourceFormat format, int32 arrayLayers = 1, int32 mipLevels = -1, const void *data = nullptr, ResourceBindFlags flags = ResourceBind::ShaderResource);
    static SPtr<Texture> Create2D(int32 width, int32 height, ResourceFormat format, int32 arrayLayers = 1, int32 mipLevels = -1, const void *data = nullptr, ResourceBindFlags flags = ResourceBind::ShaderResource);
    static SPtr<Texture> Create3D(int32 width, int32 height, int32 depth, ResourceFormat format, int32 arrayLayers = 1, int32 mipLevels = -1, const void *data = nullptr, ResourceBindFlags flags = ResourceBind::ShaderResource);
    static SPtr<Texture> CreateCube(int32 width, int32 height, ResourceFormat format, int32 arrayLayers = 1, int32 mipLevels = -1, const void *data = nullptr, ResourceBindFlags flags = ResourceBind::ShaderResource);
    static SPtr<Texture> Create2DMS(int32 width, int32 height, ResourceFormat format, int32 sampleCount, int32 arrayLayers = 1, ResourceBindFlags flags = ResourceBind::ShaderResource);

    static SPtr<Texture> Create2D(int32 width, int32 height, ResourceFormat format, void *handle, ResourceBindFlags flags = ResourceBind::ShaderResource);

protected:
    virtual void InitData(const void *data, bool autoGenMips) = 0;

    int32 GetMaxMipLevel(int32 width, int32 height, int32 depth) const;
    void CheckViewParams(int32 &mostDetailedMip, int32 &mipLevels, int32 &firstArraySlice, int32 &arrayLayers) const;

protected: 
    int32 width = 1;
    int32 height = 1;
    int32 depth = 1;
    int32 mipLevels = 1;
    int32 arrayLayers = 1;
    int32 sampleCount = 1;
    ResourceFormat format = ResourceFormat::Unknown;

    Array<SPtr<ResourceView>> srvs;
    Array<SPtr<ResourceView>> uavs;
    Array<SPtr<ResourceView>> rtvs;
    Array<SPtr<ResourceView>> dsvs;
    WPtr<Texture> weakThis;
};

}
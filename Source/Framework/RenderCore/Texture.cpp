#include "RenderCore/Texture.h"
#include "RenderCore/RenderContext.h"
#include "Core/Math.h"

namespace RenderCore
{

ResourceBindFlags UpdateBindFlags(ResourceBindFlags flags, bool hasInitData, uint32 mipLevels, ResourceFormat format)
{
    if(hasInitData && mipLevels == UINT32_MAX)
    {
        flags |= ResourceBind::RenderTarget;
    }
    return flags;
}

SPtr<Texture> Texture::Create1D(uint32 width, ResourceFormat format, uint32 arrayLayers, uint32 mipLevels, const void *data, ResourceBindFlags flags)
{
    flags = UpdateBindFlags(flags, data != nullptr, mipLevels, format);
    return Create(width, 1, 1, format, ResourceType::Texture1D, arrayLayers, mipLevels, 1, data, flags);
}

SPtr<Texture> Texture::Create2D(uint32 width, uint32 height, ResourceFormat format, uint32 arrayLayers, uint32 mipLevels, const void *data, ResourceBindFlags flags)
{
    flags = UpdateBindFlags(flags, data != nullptr, mipLevels, format);
    return Create(width, height, 1, format, ResourceType::Texture2D, arrayLayers, mipLevels, 1, data, flags);
}

SPtr<Texture> Texture::Create3D(uint32 width, uint32 height, uint32 depth, ResourceFormat format, uint32 arrayLayers, uint32 mipLevels, const void *data, ResourceBindFlags flags)
{
    flags = UpdateBindFlags(flags, data != nullptr, mipLevels, format);
    return Create(width, height, depth, format, ResourceType::Texture3D, arrayLayers, mipLevels, 1, data, flags);
}

SPtr<Texture> Texture::CreateCube(uint32 width, uint32 height, ResourceFormat format, uint32 arrayLayers, uint32 mipLevels, const void *data, ResourceBindFlags flags)
{
    flags = UpdateBindFlags(flags, data != nullptr, mipLevels, format);
    return Create(width, height, 1, format, ResourceType::TextureCube, arrayLayers, mipLevels, 1, data, flags);
}

Texture::Texture(uint32 width, uint32 height, uint32 depth, uint32 arrayLayers, uint32 mipLevels, uint32 sampleCount, ResourceFormat format, ResourceType resourceType, ResourceBindFlags flags)
    : Resource(resourceType, flags, 0), width(width), height(height), depth(depth), arrayLayers(arrayLayers), mipLevels(mipLevels), sampleCount(sampleCount), format(format)
{
    CT_CHECK(width > 0 && height > 0 && depth > 0);
    CT_CHECK(arrayLayers > 0 && mipLevels > 0 && sampleCount > 0);

    mipLevels = Math::Min(mipLevels, 1 + GetMaxMipLevel(width, height, depth));
    this->mipLevels = mipLevels;

    stateData.subStates.Add(stateData.state, arrayLayers * mipLevels); //resize and fill
}

void Texture::ClearViews()
{
    srvs.Clear();
    uavs.Clear();
    rtvs.Clear();
    dsvs.Clear();
}

SPtr<ResourceView> Texture::GetSrv(uint32 mostDetailedMip, uint32 mipLevels, uint32 firstArraySlice, uint32 arrayLayers)
{
    CheckViewParams(mostDetailedMip, mipLevels, firstArraySlice, arrayLayers);

    auto viewInfo = ResourceViewInfo(mostDetailedMip, mipLevels, firstArraySlice, arrayLayers);
    for (const auto &e : srvs)
    {
        if (e->GetViewInfo() == viewInfo)
            return e;
    }
    auto newView = ResourceView::CreateSrv(weakThis.lock(), mostDetailedMip, mipLevels, firstArraySlice, arrayLayers);
    srvs.Add(newView);
    return newView;
}

SPtr<ResourceView> Texture::GetUav(uint32 mipLevel, uint32 firstArraySlice, uint32 arrayLayers)
{
    uint32 mostDetailedMip = mipLevel;
    uint32 mipLevels = 1;

    CheckViewParams(mostDetailedMip, mipLevels, firstArraySlice, arrayLayers);

    auto viewInfo = ResourceViewInfo(mostDetailedMip, mipLevels, firstArraySlice, arrayLayers);
    for (const auto &e : uavs)
    {
        if (e->GetViewInfo() == viewInfo)
            return e;
    }
    auto newView = ResourceView::CreateUav(weakThis.lock(), mostDetailedMip, firstArraySlice, arrayLayers);
    uavs.Add(newView);
    return newView;
}

SPtr<ResourceView> Texture::GetRtv(uint32 mipLevel, uint32 firstArraySlice, uint32 arrayLayers)
{
    uint32 mostDetailedMip = mipLevel;
    uint32 mipLevels = 1;

    CheckViewParams(mostDetailedMip, mipLevels, firstArraySlice, arrayLayers);

    auto viewInfo = ResourceViewInfo(mostDetailedMip, mipLevels, firstArraySlice, arrayLayers);
    for (const auto &e : rtvs)
    {
        if (e->GetViewInfo() == viewInfo)
            return e;
    }
    auto newView = ResourceView::CreateRtv(weakThis.lock(), mostDetailedMip, firstArraySlice, arrayLayers);
    rtvs.Add(newView);
    return newView;
}

SPtr<ResourceView> Texture::GetDsv(uint32 mipLevel, uint32 firstArraySlice, uint32 arrayLayers)
{
    uint32 mostDetailedMip = mipLevel;
    uint32 mipLevels = 1;

    CheckViewParams(mostDetailedMip, mipLevels, firstArraySlice, arrayLayers);

    auto viewInfo = ResourceViewInfo(mostDetailedMip, mipLevels, firstArraySlice, arrayLayers);
    for (const auto &e : dsvs)
    {
        if (e->GetViewInfo() == viewInfo)
            return e;
    }
    auto newView = ResourceView::CreateDsv(weakThis.lock(), mostDetailedMip, firstArraySlice, arrayLayers);
    dsvs.Add(newView);
    return newView;
}

uint32 GetMaxMipLevel(uint32 width, uint32 height, uint32 depth)
{
    auto dim = Math::Max(width, height, depth);
    return (uint32)Math::Log2((float)dim);
}

void Texture::CheckViewParams(uint32 &mostDetailedMip, uint32 &mipLevels, uint32 &firstArraySlice, uint32 &arrayLayers) const
{
    const uint32 texMipLevels = this->mipLevels;
    const uint32 texArrayLayers = this->arrayLayers;

    if (mostDetailedMip >= texMipLevels)
    {
        CT_LOG(Warning, CT_TEXT("Param mostDetailedMip is valid, texture mipLevels is {0}."), texMipLevels);
        mostDetailedMip = mipLevels - 1;
    }

    if (mipLevels == UINT32_MAX)
    {
        mipLevels = texMipLevels - mostDetailedMip;
    }
    else if (mipLevels + mostDetailedMip > texMipLevels)
    {
        CT_LOG(Warning, CT_TEXT("Param mipLevels is valid, texture mipLevels is {0}."), texMipLevels);
        mipLevels = texMipLevels - mostDetailedMip;
    }

    if (firstArraySlice >= texArrayLayers)
    {
        CT_LOG(Warning, CT_TEXT("Param firstArraySlice is valid, texture arrayLayers is {0}."), texArrayLayers);
        firstArraySlice = texArrayLayers - 1;
    }

    if (arrayLayers == UINT32_MAX)
    {
        arrayLayers = texArrayLayers - firstArraySlice;
    }
    else if (arrayLayers + firstArraySlice > texArrayLayers)
    {
        CT_LOG(Warning, CT_TEXT("Param arrayLayers is valid, texture arrayLayers is {0}."), texArrayLayers);
        arrayLayers = texArrayLayers - firstArraySlice;
    }
}

void Texture::GenerateMips(RenderContext *ctx)
{
    for (uint32 m = 0; m < mipLevels - 1; ++m)
    {
        for (uint32 a = 0; a < arrayLayers; ++a)
        {
            auto srv = GetSrv(m, 1, a, 1);
            auto rtv = GetRtv(m + 1, a, 1);
            ctx->Blit(srv.get(), rtv.get());
        }
    }

    //TODO Release views ?
}

}
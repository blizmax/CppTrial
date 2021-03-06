#include "RenderCore/Texture.h"
#include "Core/Math.h"
#include "RenderCore/RenderContext.h"

int32 Texture::sRuntimeID = 0;

ResourceBindFlags UpdateBindFlags(ResourceBindFlags flags, bool hasInitData, int32 mipLevels, ResourceFormat format)
{
    if (hasInitData && mipLevels == -1)
    {
        flags |= ResourceBind::RenderTarget;
    }

    auto supported = GetResourceFormatBindFlags(format);
    supported |= ResourceBind::Shared;
    if ((flags & supported) != flags)
    {
        CT_EXCEPTION(RenderCore, "Texture::CreateXX() requested ResourceBindFlags not supported.");
        flags = flags & supported;
    }

    return flags;
}

SPtr<Texture> Texture::Create1D(int32 width, ResourceFormat format, int32 arrayLayers, int32 mipLevels, const void *data, ResourceBindFlags flags)
{
    flags = UpdateBindFlags(flags, data != nullptr, mipLevels, format);
    return Create(width, 1, 1, format, ResourceType::Texture1D, arrayLayers, mipLevels, 1, data, flags);
}

SPtr<Texture> Texture::Create2D(int32 width, int32 height, ResourceFormat format, int32 arrayLayers, int32 mipLevels, const void *data, ResourceBindFlags flags)
{
    flags = UpdateBindFlags(flags, data != nullptr, mipLevels, format);
    return Create(width, height, 1, format, ResourceType::Texture2D, arrayLayers, mipLevels, 1, data, flags);
}

SPtr<Texture> Texture::Create3D(int32 width, int32 height, int32 depth, ResourceFormat format, int32 mipLevels, const void *data, ResourceBindFlags flags)
{
    flags = UpdateBindFlags(flags, data != nullptr, mipLevels, format);
    return Create(width, height, depth, format, ResourceType::Texture3D, 1, mipLevels, 1, data, flags);
}

SPtr<Texture> Texture::CreateCube(int32 width, int32 height, ResourceFormat format, int32 arrayLayers, int32 mipLevels, const void *data, ResourceBindFlags flags)
{
    flags = UpdateBindFlags(flags, data != nullptr, mipLevels, format);
    return Create(width, height, 1, format, ResourceType::TextureCube, arrayLayers, mipLevels, 1, data, flags);
}

SPtr<Texture> Texture::Create2DMS(int32 width, int32 height, ResourceFormat format, int32 sampleCount, int32 arrayLayers, ResourceBindFlags flags)
{
    flags = UpdateBindFlags(flags, false, 1, format);
    return Create(width, height, 1, format, ResourceType::Texture2DMultisample, arrayLayers, 1, sampleCount, nullptr, flags);
}

Texture::Texture(int32 width, int32 height, int32 depth, int32 arrayLayers, int32 mipLevels, int32 sampleCount, ResourceFormat format, ResourceType resourceType, ResourceBindFlags flags)
    : Resource(resourceType, flags, 0), width(width), height(height), depth(depth), arrayLayers(arrayLayers), mipLevels(mipLevels), sampleCount(sampleCount), format(format)
{
    CT_CHECK(width > 0 && height > 0 && depth > 0);

    auto totalMipLevels = 1 + GetMaxMipLevel(width, height, depth);
    if (mipLevels == -1)
        mipLevels = totalMipLevels;
    else
        mipLevels = Math::Min(mipLevels, totalMipLevels);
    this->mipLevels = mipLevels;

    CT_CHECK(arrayLayers > 0 && mipLevels > 0 && sampleCount > 0);

    stateData.subStates.Add(stateData.state, arrayLayers * mipLevels); //resize and fill

    runtimeID = sRuntimeID++;
}

void Texture::ClearViews()
{
    srvs.Clear();
    uavs.Clear();
    rtvs.Clear();
    dsvs.Clear();
}

SPtr<ResourceView> Texture::GetSrv(int32 mostDetailedMip, int32 mipLevels, int32 firstArraySlice, int32 arrayLayers)
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

SPtr<ResourceView> Texture::GetUav(int32 mipLevel, int32 firstArraySlice, int32 arrayLayers)
{
    int32 mostDetailedMip = mipLevel;
    int32 mipLevels = 1;

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

SPtr<ResourceView> Texture::GetRtv(int32 mipLevel, int32 firstArraySlice, int32 arrayLayers)
{
    int32 mostDetailedMip = mipLevel;
    int32 mipLevels = 1;

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

SPtr<ResourceView> Texture::GetDsv(int32 mipLevel, int32 firstArraySlice, int32 arrayLayers)
{
    int32 mostDetailedMip = mipLevel;
    int32 mipLevels = 1;

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

int32 Texture::GetMaxMipLevel(int32 width, int32 height, int32 depth) const
{
    auto dim = Math::Max(width, height, depth);
    return (int32)Math::Log2((float)dim);
}

void Texture::CheckViewParams(int32 &mostDetailedMip, int32 &mipLevels, int32 &firstArraySlice, int32 &arrayLayers) const
{
    const int32 texMipLevels = this->mipLevels;
    const int32 texArrayLayers = this->arrayLayers;

    if (mostDetailedMip >= texMipLevels)
    {
        CT_LOG(Warning, CT_TEXT("Param mostDetailedMip is valid, texture mipLevels is {0}."), texMipLevels);
        mostDetailedMip = mipLevels - 1;
    }

    if (mipLevels == -1)
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

    if (arrayLayers == -1)
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
    SetSubresourceState(0, 0, GetGlobalState()); //NOTE Init sub state for subresource barrier

    for (int32 m = 0; m < mipLevels - 1; ++m)
    {
        for (int32 a = 0; a < arrayLayers; ++a)
        {
            auto srv = GetSrv(m, 1, a, 1);
            auto rtv = GetRtv(m + 1, a, 1);
            ctx->Blit(srv.get(), rtv.get());
        }
    }

    //TODO Release views ?
}
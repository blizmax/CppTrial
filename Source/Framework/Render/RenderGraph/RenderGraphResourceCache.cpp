#include "Render/RenderGraph/RenderGraphResourceCache.h"

SPtr<RenderGraphResourceCache> RenderGraphResourceCache::Create()
{
    return Memory::MakeShared<RenderGraphResourceCache>();
}

void RenderGraphResourceCache::Reset()
{
    nameToIndex.Clear();
    resourceDatas.Clear();
}

void RenderGraphResourceCache::RegisterExternalResource(const String &name, const SPtr<Resource> &resource)
{
    externalResources.Put(name, resource);
}

void RenderGraphResourceCache::RegisterField(const String &name, const RenderPassReflection::Field &field)
{
    if (nameToIndex.Contains(name))
    {
        CT_LOG(Error, CT_TEXT("RenderGraphResourceCache::RegisterField() try to register an existed field, name: {0}."), name);
        return;
    }

    int32 newIndex = resourceDatas.Count();
    nameToIndex.Put(name, newIndex);
    bool resolveBindFlags = field.GetResourceBindFlags() == ResourceBind::None;

    resourceDatas.Add({ .resource = nullptr, .resolveBindFlags = resolveBindFlags });
}

SPtr<Resource> RenderGraphResourceCache::GetResource(const String &name) const
{
    auto pRes = externalResources.TryGet(name);
    if (pRes)
        return *pRes;

    auto pIdx = nameToIndex.TryGet(name);
    if (pIdx)
        return resourceDatas[*pIdx].resource;

    return nullptr;
}

const RenderPassReflection::Field &RenderGraphResourceCache::GetResourceReflection(const String &name) const
{
    auto ptr = nameToIndex.TryGet(name);
    CT_CHECK(ptr);
    return resourceDatas[*ptr].field;
}

void RenderGraphResourceCache::AllocateResources(const RenderGraph::ResourceProperties &defaultProps)
{
    auto CreateResource = [&, this](ResourceData &r) {
        auto &field = r.field;

        auto width = field.GetWidth() ? field.GetWidth() : defaultProps.width;
        auto height = field.GetHeight() ? field.GetHeight() : defaultProps.height;
        auto depth = field.GetDepth() ? field.GetDepth() : 1;
        auto sampleCount = field.GetSampleCount() ? field.GetSampleCount() : 1;
        auto bindFlags = field.GetResourceBindFlags();
        auto arrayLayers = field.GetArrayLayers();
        auto mipLevels = field.GetMipLevels();

        ResourceFormat format = ResourceFormat::Unknown;

        if (field.GetFieldType() != RenderPassReflection::FieldType::RawBuffer)
        {
            format = field.GetResourceFormat() != ResourceFormat::Unknown ? field.GetResourceFormat() : defaultProps.format;
            if (r.resolveBindFlags)
            {
                ResourceBindFlags flags = ResourceBind::UnorderedAccess | ResourceBind::ShaderResource;
                if (field.IsOutput() || field.IsInternal())
                    flags |= (ResourceBind::DepthStencil | ResourceBind::RenderTarget);
                auto supported = GetResourceFormatBindFlags(format);
                flags &= supported;
                bindFlags |= flags;
            }
        }
        else
        {
            if (r.resolveBindFlags)
            {
                bindFlags = ResourceBind::UnorderedAccess | ResourceBind::ShaderResource;
            }
        }

        SPtr<Resource> resource;
        switch (field.GetFieldType())
        {
        case RenderPassReflection::FieldType::RawBuffer:
            resource = Buffer::Create(width, bindFlags, BufferCpuAccess::None);
            break;
        case RenderPassReflection::FieldType::Texture1D:
            resource = Texture::Create1D(width, format, arrayLayers, mipLevels, nullptr, bindFlags);
            break;
        case RenderPassReflection::FieldType::Texture2D:
            if (sampleCount > 1)
                resource = Texture::Create2DMS(width, height, format, sampleCount, arrayLayers, bindFlags);
            else
                resource = Texture::Create2D(width, height, format, arrayLayers, mipLevels, nullptr, bindFlags);
            break;
        case RenderPassReflection::FieldType::Texture3D:
            resource = Texture::Create3D(width, height, depth, format, mipLevels, nullptr, bindFlags);
            break;
        case RenderPassReflection::FieldType::TextureCube:
            resource = Texture::CreateCube(width, height, format, arrayLayers, mipLevels, nullptr, bindFlags);
            break;
        default:
            CT_CHECK(false);
            break;
        }

        r.resource = resource;
    };


    for (auto &e : resourceDatas)
    {
        if ((e.resource == nullptr) && (e.field.IsValid()))
        {
            CreateResource(e);
        }
    }
}
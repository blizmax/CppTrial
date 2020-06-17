#include "RenderCore/FrameBuffer.h"
#include "Core/Math.h"

namespace RenderCore
{

static ResourceBindFlags GetBindFlags(bool isDepth, bool allowUav)
{
    ResourceBindFlags flags = ResourceBind::ShaderResource;
    flags |= isDepth ? ResourceBind::DepthStencil : ResourceBind::RenderTarget;
    if (allowUav)
    {
        flags |= ResourceBind::UnorderedAccess;
    }
    return flags;
}

SPtr<Texture> CreateTexture2D(int32 w, int32 h, ResourceFormat format, int32 sampleCount, int32 arrayLayers, int32 mipLevels, ResourceBindFlags flags)
{
    CT_CHECK(format != ResourceFormat::Unknown);

    if (sampleCount > 1)
    {
        return Texture::Create2DMS(w, h, format, sampleCount, arrayLayers, flags);
    }
    else
    {
        return Texture::Create2D(w, h, format, arrayLayers, mipLevels, nullptr, flags);
    }
}

SPtr<FrameBuffer> FrameBuffer::Create(const Array<SPtr<Texture>> &colors, const SPtr<Texture> &depthStencil)
{
    auto ptr = FrameBuffer::Create();

    for(const auto &e : colors)
    {
        ptr->AddColorAttachment(e);
    }
    if(depthStencil)
    {
        ptr->SetDepthStencilAttachment(depthStencil);
    }
    ptr->Apply();

    return ptr;
}

SPtr<FrameBuffer> FrameBuffer::Create2D(int32 width, int32 height, const FrameBufferDesc &desc, int32 arrayLayers, int32 mipLevels)
{
    CT_CHECK(width > 0);
    CT_CHECK(height > 0);
    CT_CHECK(arrayLayers > 0);
    CT_CHECK(mipLevels > 0);
    CT_CHECK(desc.sampleCount >= 0);
    if(desc.sampleCount > 1)
        CT_CHECK(mipLevels == 1);

    auto ptr = FrameBuffer::Create();
    for(const auto &e : desc.colors)
    {
        ResourceBindFlags flags = GetBindFlags(false, e.allowUav);
        auto texture = CreateTexture2D(width, height, e.format, desc.sampleCount, arrayLayers, mipLevels, flags);
        ptr->AddColorAttachment(texture);
    }
    if(desc.hasDepthStencil)
    {
        ResourceBindFlags flags = GetBindFlags(true, desc.depthStencil.allowUav);
        auto texture = CreateTexture2D(width, height, desc.depthStencil.format, desc.sampleCount, arrayLayers, mipLevels, flags);
        ptr->SetDepthStencilAttachment(texture);
    }
    ptr->Apply();

    return ptr;
}

SPtr<FrameBuffer> FrameBuffer::CreateCubemap(int32 width, int32 height, const FrameBufferDesc& desc, int32 arrayLayers, int32 mipLevels)
{
    CT_CHECK(width > 0);
    CT_CHECK(height > 0);
    CT_CHECK(arrayLayers > 0);
    CT_CHECK(mipLevels > 0);
    CT_CHECK(desc.sampleCount == 1);

    auto ptr = FrameBuffer::Create();
    for(const auto &e : desc.colors)
    {
        ResourceBindFlags flags = GetBindFlags(false, e.allowUav);
        auto texture = Texture::CreateCube(width, height, e.format, arrayLayers, mipLevels, nullptr, flags);
        ptr->AddColorAttachment(texture);
    }
    if(desc.hasDepthStencil)
    {
        ResourceBindFlags flags = GetBindFlags(true, desc.depthStencil.allowUav);
        auto texture = Texture::CreateCube(width, height, desc.depthStencil.format, arrayLayers, mipLevels, nullptr, flags);
        ptr->SetDepthStencilAttachment(texture);
    }
    ptr->Apply();

    return ptr;
}

void FrameBuffer::AddColorAttachment(const SPtr<Texture> &texture, int32 mipLevel, int32 firstArraySlice, int32 arrayLayers)
{
    if(colorAttachments.Count() >= COLOR_ATTCHMENT_MAX_NUM)
    {
        CT_LOG(Error, CT_TEXT("Add color attachment failed, index out of bound, max count is {0}."), COLOR_ATTCHMENT_MAX_NUM);
        return;
    }

    //TODO Check params
    Attachment attachment = {};
    attachment.texture = texture;
    attachment.mipLevel = mipLevel;
    attachment.arrayLayers = arrayLayers;
    attachment.firstArraySlice = firstArraySlice;
    colorAttachments.Add(attachment);

    FrameBufferDesc::AttachmentDesc attachmentDesc = {};
    attachmentDesc.format = texture->GetResourceFormat();
    desc.colors.Add(attachmentDesc);
}

void FrameBuffer::SetDepthStencilAttachment(const SPtr<Texture> &texture, int32 mipLevel, int32 firstArraySlice, int32 arrayLayers)
{
    //TODO Check params
    Attachment attachment = {};
    attachment.texture = texture;
    attachment.mipLevel = mipLevel;
    attachment.arrayLayers = arrayLayers;
    attachment.firstArraySlice = firstArraySlice;
    depthStencilAttachment = attachment;

    FrameBufferDesc::AttachmentDesc attachmentDesc = {};
    attachmentDesc.format = texture->GetResourceFormat();
    desc.depthStencil = attachmentDesc;

    desc.hasDepthStencil = true;
}

void FrameBuffer::Apply()
{
    desc.sampleCount = colorAttachments[0].texture->GetSampleCount();
    layered = colorAttachments[0].arrayLayers > 1;
    width = colorAttachments[0].texture->GetWidth(colorAttachments[0].mipLevel);
    height = colorAttachments[0].texture->GetHeight(colorAttachments[0].mipLevel);
    depth = colorAttachments[0].texture->GetDepth(colorAttachments[0].mipLevel);

    auto VerifyAttchment = [this](Attachment &a)
    {
        if(desc.sampleCount != a.texture->GetSampleCount())
            return false;
        if(layered != (a.arrayLayers > 1))
            return false;
        if(width != a.texture->GetWidth(a.mipLevel))
            return false;
        if(height != a.texture->GetHeight(a.mipLevel))
            return false;
        if(depth != a.texture->GetDepth(a.mipLevel))
            return false;
        return true;
    };

    for(int32 i = 1; i < colorAttachments.Count(); ++i)
    {
        if(!VerifyAttchment(colorAttachments[i]))
        {
            CT_EXCEPTION(RenderCore, "Color attachment is valid.");
        }
    }

    if(desc.hasDepthStencil && !VerifyAttchment(depthStencilAttachment))
    {
        CT_EXCEPTION(RenderCore, "Depthstencil attachment is valid.");
    }
}

}
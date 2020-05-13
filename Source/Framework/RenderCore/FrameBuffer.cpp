#include "RenderCore/FrameBuffer.h"
#include "Core/Math.h"

namespace RenderCore
{

void FrameBuffer::AddColorAttachment(const SPtr<Texture> &texture, uint32 mipLevel, uint32 firstArraySlice, uint32 arrayLayers)
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

void FrameBuffer::SetDepthStencilAttachment(const SPtr<Texture> &texture, uint32 mipLevel, uint32 firstArraySlice, uint32 arrayLayers)
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
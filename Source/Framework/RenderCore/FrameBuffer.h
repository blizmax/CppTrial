#pragma once

#include "RenderCore/Texture.h"

namespace RenderCore
{
struct FrameBufferDesc
{
    struct AttachmentDesc
    {
        ResourceFormat format = ResourceFormat::Unknown;
        bool allowUav = false;

        AttachmentDesc(ResourceFormat format, bool allowUav = false) : format(format), allowUav(allowUav)
        {
        }
    };

    Array<AttachmentDesc> colors;
    AttachmentDesc depthStencil;
    uint32 sampleCount = 1;
};

class FrameBuffer
{
public:
    struct Attachment
    {
        SPtr<Texture> texture = nullptr;
        uint32 mipLevel = 0;
        uint32 arrayLayers = 1;
        uint32 firstArraySlice = 0;
    };

    virtual ~FrameBuffer() = default;

    virtual void Apply();

    void AddColorAttachment(const SPtr<Texture> &texture, uint32 mipLevel = 0, uint32 firstArraySlice = 0, uint32 arrayLayers = UINT32_MAX);
    void SetDepthStencilAttachment(const SPtr<Texture> &texture, uint32 mipLevel = 0, uint32 firstArraySlice = 0, uint32 arrayLayers = UINT32_MAX);

    uint32 GetWidth() const
    {
        return width;
    }

    uint32 GetHeight() const
    {
        return height;
    }

    uint32 GetDepth() const
    {
        return depth;
    }

    const SPtr<Texture> &GetColorTexture(uint32 index) const
    {
        CT_CHECK(index < COLOR_ATTCHMENT_MAX_NUM);
        return colorAttachments[index].texture;
    }

    const SPtr<Texture> &GetDepthStencilTexture() const
    {
        return depthStencilAttachment.texture;
    }

    SPtr<ResourceView> GetRtv(uint32 index) const
    {
        CT_CHECK(index < COLOR_ATTCHMENT_MAX_NUM);
        const auto &a = colorAttachments[index];
        if (a.texture)
        {
            return a.texture->GetSrv(a.mipLevel, a.firstArraySlice, a.arrayLayers);
        }
        return nullptr;
    }

    SPtr<ResourceView> GetDsv() const
    {
        const auto &a = depthStencilAttachment;
        if (a.texture)
        {
            return a.texture->GetSrv(a.mipLevel, a.firstArraySlice, a.arrayLayers);
        }
        return nullptr;
    }

    static SPtr<FrameBuffer> Create(const Array<SPtr<Texture>> &colors, const SPtr<Texture> &depthStencil = nullptr);

protected:
    uint32 width;
    uint32 height;
    uint32 depth;
    Array<Attachment> colorAttachments;
    Attachment depthStencilAttachment;
    FrameBufferDesc desc;
    bool layered;
    bool hasDepthStencil = false;
};
}
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

        AttachmentDesc() = default;
        AttachmentDesc(ResourceFormat format, bool allowUav = false) : format(format), allowUav(allowUav)
        {
        }

        bool operator==(const AttachmentDesc &other) const
        {
            return format == other.format && allowUav == other.allowUav;
        }

        bool operator!=(const AttachmentDesc &other) const
        {
            return format != other.format || allowUav != other.allowUav;
        }
    };

    Array<AttachmentDesc> colors;
    AttachmentDesc depthStencil;
    uint32 sampleCount = 1;
    bool hasDepthStencil = false;
    void *renderPass = nullptr;

    bool operator==(const FrameBufferDesc &other) const
    {
        if(sampleCount != other.sampleCount)
            return false;
        if(renderPass != other.renderPass)
            return false;
        if(hasDepthStencil != other.hasDepthStencil)
            return false;
        if(hasDepthStencil && (depthStencil != other.depthStencil))
            return false;
        if(colors != other.colors)
            return false;
        return true;
    }

    bool operator!=(const FrameBufferDesc &other) const
    {
        return !(*this == other); 
    }
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

    uint32 GetSampleCount() const
    {
        return desc.sampleCount;
    }

    const FrameBufferDesc &GetDesc() const
    {
        return desc;
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
            return a.texture->GetRtv(a.mipLevel, a.firstArraySlice, a.arrayLayers);
        }
        return nullptr;
    }

    SPtr<ResourceView> GetDsv() const
    {
        const auto &a = depthStencilAttachment;
        if (a.texture)
        {
            return a.texture->GetDsv(a.mipLevel, a.firstArraySlice, a.arrayLayers);
        }
        return nullptr;
    }

    static SPtr<FrameBuffer> Create();
    static SPtr<FrameBuffer> Create(const Array<SPtr<Texture>> &colors, const SPtr<Texture> &depthStencil = nullptr);
    static SPtr<FrameBuffer> Create2D(uint32 width, uint32 height, const FrameBufferDesc &desc, uint32 arrayLayers = 1, uint32 mipLevels = 1);
    static SPtr<FrameBuffer> CreateCubemap(uint32 width, uint32 height, const FrameBufferDesc& desc, uint32 arrayLayers = 1, uint32 mipLevels = 1);

protected:
    uint32 width;
    uint32 height;
    uint32 depth;
    Array<Attachment> colorAttachments;
    Attachment depthStencilAttachment;
    FrameBufferDesc desc;
    bool layered;
};
}
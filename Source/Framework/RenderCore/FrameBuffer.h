#pragma once

#include "RenderCore/Texture.h"

namespace RenderCore
{
struct FrameBufferDesc
{
    struct AttachmentDesc
    {
        ResourceFormat format = ResourceFormat::Unknown;
    };

    Array<AttachmentDesc> colors;
    AttachmentDesc depthStencil;
    uint32 sampleCount = 0;
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
        return colorAttachments[index].texture;
    }

    const SPtr<Texture> &GetDepthStencilTexture() const
    {
        return depthStencilAttachment.texture;
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
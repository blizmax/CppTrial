#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

struct BlendStateDesc
{
    struct AttachmentDesc
    {
        bool enabled = false;
        uint8 writeMask = 0xFF; // r:1, g:2, b:4, a:8
        BlendFactor srcFactor = BlendFactor::SrcAlpha;
        BlendFactor dstFactor = BlendFactor::OneMinusSrcAlpha;
        BlendOperation blendOp = BlendOperation::Add;
        BlendFactor srcAlphaFactor = BlendFactor::One;
        BlendFactor dstAlphaFactor = BlendFactor::Zero;
        BlendOperation alphaBlendOp = BlendOperation::Add;
    };

    AttachmentDesc attachments[COLOR_ATTCHMENT_MAX_NUM];
};

class BlendState
{
public:
    BlendState(const BlendStateDesc &desc) : desc(desc)
    {
    }

    const BlendStateDesc &GetDesc() const
    {
        return desc;
    }

    const BlendStateDesc::AttachmentDesc &GetAttachmentDesc(int32 i) const
    {
        CT_CHECK(i >= 0 && i < COLOR_ATTCHMENT_MAX_NUM);
        return desc.attachments[i];
    }

    static SPtr<BlendState> Create(const BlendStateDesc &desc)
    {
        return Memory::MakeShared<BlendState>(desc);
    }

private:
    BlendStateDesc desc;
};
}
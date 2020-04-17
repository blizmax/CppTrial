#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
struct BlendStateAttachmentData
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

struct BlendStateData
{
    BlendStateAttachmentData attachments[COLOR_ATTCHMENT_MAX_NUM];
};

using BlendStateCreateParams = BlendStateData;

class BlendState
{
public:
    BlendState(const BlendStateCreateParams &params) : data(params)
    {
    }

    auto GetData() const
    {
        return data;
    }

    auto GetAttachmentData(int32 i) const
    {
        CT_CHECK(i >= 0 && i < COLOR_ATTCHMENT_MAX_NUM);
        return data.attachments[i];
    }

    static SPtr<BlendState> Create(const BlendStateCreateParams &params)
    {
        return Memory::MakeShared<BlendState>(params);
    }

private:
    BlendStateData data;
};
}
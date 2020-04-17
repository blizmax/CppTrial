#pragma once

#include "RenderCore/RenderAPI.h"
#include "RenderVulkan/VulkanDevice.h"

namespace RenderCore
{
class VulkanRenderAPI : public RenderAPI
{
public:
    virtual void Init() override;
    virtual void Destroy() override;
};   
}
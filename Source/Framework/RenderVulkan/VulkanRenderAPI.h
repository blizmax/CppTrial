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

    virtual void SetRenderPipeline(const SPtr<RenderPipeline> &pipeline) override;    

    VkInstance GetInstanceHandle() const
    {
        return instance;
    }

    const UPtr<VulkanDevice> &GetDevice() const
    {
        return device;
    }

private:
    void CreateInstance();
    void CreateDebugger();
    void CreateDevice();

private:    
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    UPtr<VulkanDevice> device;

    bool enableValidationLayers = false;
};   
}
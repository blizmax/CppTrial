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

private:
    void GetExtensions(Array<const char8 *> &result);
    void GetValidationLayers(Array<const char8 *> &result);

    void CreateInstance();
    void CreateDebugger();
    void CreateDevice();

private:    
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    SPtr<VulkanDevice> device;

    bool enableValidationLayers = false;
};   
}
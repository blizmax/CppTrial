#pragma once

#include "RenderVulkan/VulkanMemory.h"

namespace RenderCore
{

struct VulkanImageCreateParams
{
    VkImage image;
    VkFormat format;
    VkImageLayout layout;
};
 
class VulkanImage : public IVulkanResource
{
public:
    VulkanImage(const VulkanImageCreateParams &params, bool ownsImage);
    ~VulkanImage();

    virtual void Destroy() override;

    static SPtr<VulkanImage> Create(const VulkanImageCreateParams &params, bool ownsImage = true);

private:
    void CreateImageView(const VulkanImageCreateParams &params);
private:
    VkImage image = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;
    bool ownsImage;
};
}
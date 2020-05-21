#pragma once

#include "RenderVulkan/VulkanMemory.h"
#include "RenderCore/Texture.h"

namespace RenderCore
{
class VulkanTexture : public Texture
{
public:
    struct TextureData
    {
        VkImage image = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
    };

    VulkanTexture(uint32 width, uint32 height, uint32 depth, uint32 arrayLayers, uint32 mipLevels, uint32 sampleCount, ResourceFormat format, ResourceType resourceType, ResourceBindFlags flags);

    virtual ~VulkanTexture();

    VkImage GetHandle() const
    {
        return textureData.image;
    }

protected:
    friend class Texture;
    virtual void InitData(const void *data, bool autoGenMips) override;

private:
    TextureData textureData{};
    bool ownsImage = true;
};
}
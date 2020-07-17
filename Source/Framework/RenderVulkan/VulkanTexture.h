#pragma once

#include "RenderCore/Texture.h"
#include "RenderVulkan/VulkanMemory.h"

class VulkanTexture : public Texture
{
public:
    struct TextureData
    {
        VkImage image = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
    };

    VulkanTexture(int32 width, int32 height, int32 depth, int32 arrayLayers, int32 mipLevels, int32 sampleCount, ResourceFormat format, ResourceType resourceType, ResourceBindFlags flags);

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
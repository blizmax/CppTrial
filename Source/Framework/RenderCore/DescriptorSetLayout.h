#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{

struct DescriptorSetLayoutDesc
{
    struct Element
    {
        String name;
        DescriptorType descriptorType = DescriptorType::Unknown;
        uint32 binding = 0;
        uint32 arrayLength = 1;
    };

    Array<Element> elements;
    ShaderVisibilityFlags visibility = ShaderVisibility::All;
    uint32 setIndex = 0;
};

class DescriptorSetLayout
{
public:
    DescriptorSetLayout(const DescriptorSetLayoutDesc &desc) : desc(desc)
    {
    }

    virtual ~DescriptorSetLayout() = default;

    const DescriptorSetLayoutDesc &GetDesc() const
    {
        return desc;
    }

    const Array<DescriptorSetLayoutDesc::Element> &GetElements() const
    {
        return desc.elements;
    }

    ShaderVisibilityFlags GetVisibility() const
    {
        return desc.visibility;
    }

    static SPtr<DescriptorSetLayout> Create(const DescriptorSetLayoutDesc &desc);

protected:
    DescriptorSetLayoutDesc desc;
};

}
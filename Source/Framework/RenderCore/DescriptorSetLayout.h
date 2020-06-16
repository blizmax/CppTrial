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
        int32 binding = 0;
        int32 arrayLength = 1;
    };

    Array<Element> elements;
    ShaderVisibilityFlags visibility = ShaderVisibility::All;
    int32 setIndex = 0;
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

    const DescriptorSetLayoutDesc::Element &GetElement(int32 binding) const
    {
        return desc.elements[binding];
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
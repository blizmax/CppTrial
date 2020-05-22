#pragma once

#include "RenderCore/.Package.h"

namespace RenderCore
{
class DescriptorSetLayout
{
public:
    struct Element
    {
        String name;
        DescriptorType descriptorType;

        Element(const String &name, DescriptorType type)
            : name(name), descriptorType(type)
        {
        }
    };

    virtual ~DescriptorSetLayout() = default;

    const Array<Element> &GetElements() const
    {
        return elements;
    }

    ShaderVisibilityFlags GetVisibility() const
    {
        return visibility;
    }

    static SPtr<DescriptorSetLayout> Create(std::initializer_list<Element> initList, ShaderVisibilityFlags visibility = ShaderVisibility::All);

protected:
    DescriptorSetLayout(std::initializer_list<Element> initList, ShaderVisibilityFlags visibility)
        : elements(initList), visibility(visibility)
    {
    }

protected:
    Array<Element> elements;
    ShaderVisibilityFlags visibility = ShaderVisibility::All;
};

}
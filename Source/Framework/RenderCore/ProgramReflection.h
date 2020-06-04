#pragma once

#include "RenderCore/DescriptorSetLayout.h"
#include "Core/HashMap.h"

namespace RenderCore
{
class ProgramReflection
{
public:
    struct BindingData
    {
        int32 index = -1;  
        String name;
        DescriptorType descriptorType = DescriptorType::Unknown;
        uint32 binding = 0;
        uint32 arrayLength = 1;
        uint32 set = 0;
    };

    struct SetData
    {
        Array<int32> bindings;
        ShaderVisibilityFlags visibility = ShaderVisibility::All;
    };

    ProgramReflection() = default;

    DescriptorSetLayoutDesc GetDescriptorSetLayoutDesc(int32 setIndex) const;

    void AddBindingData(const String &name, DescriptorType descriptorType, uint32 binding, uint32 arrayLength = 1, uint32 setIndex = 0)
    {
        bindingDatas.Add({bindingDatas.Count(), name, descriptorType, binding, arrayLength, setIndex});
        const int32 bindingDataIndex = bindingDatas.Count() - 1;

        if (setDatas.Count() <= setIndex)
        {
            setDatas.SetCount(setIndex + 1);
        }

        if (setDatas[setIndex].bindings.Count() <= binding)
        {
            setDatas[setIndex].bindings.Add(-1, binding - setDatas[setIndex].bindings.Count() + 1);
        }
        setDatas[setIndex].bindings[binding] = bindingDataIndex;
        bindingNames.Put(name, bindingDataIndex);
    }

    const BindingData &GetBindingData(const String &name) const
    {
        static const BindingData INVALID = {};
        
        auto *ptr = bindingNames.TryGet(name);
        if (!ptr)
            return INVALID;
        return bindingDatas[*ptr];
    }

    int32 GetDescriptorSetCount() const
    {
        return setDatas.Count();
    }

    static SPtr<ProgramReflection> Create()
    {
        return Memory::MakeShared<ProgramReflection>();
    }

private:
    Array<BindingData> bindingDatas;
    Array<SetData> setDatas;
    HashMap<String, int32> bindingNames;
};


}
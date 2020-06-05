#pragma once

#include "RenderCore/RootSignature.h"
#include "Core/HashMap.h"

namespace RenderCore
{

enum class ShaderDataType
{
    Bool,
    BVec2,
    BVec3,
    BVec4,
    Int,
    IVec2,
    IVec3,
    IVec4,
    UInt,
    UVec2,
    UVec3,
    UVec4,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Mat2,
    Mat2x3,
    Mat2x4,
    Mat3x2,
    Mat3,
    Mat3x4,
    Mat4x2,
    Mat4x3,
    Mat4,

    Count,
    Unknown = -1,
};

enum class ShaderResourceType
{
    Sampler1D,
    Sampler2D,
    Sampler3D,
    SamplerCube,
    Sampler2DMS,

    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
    Texture2DMS,
    Texture1DArray,
    Texture2DArray,
    TextureCubeArray,
    Texture2DMSArray,

    StructuredBuffer,
    RawBuffer,
    TypedBuffer,
    ConstantBuffer,

    Count,
    Unknown = -1,
};

enum class ShaderAccess
{
    Undefined,
    Read,
    ReadWrite,
};

// struct UniformVarOffset
// {
//     uint32 byteOffset;
// };

// struct ResourceVarOffset
// {
//     uint32 binding;
//     uint32 arrayIndex;
// };

class ReflectionTypeBase
{
public:
    virtual ~ReflectionTypeBase() = default;

    virtual bool IsArray() const {return false;}
    virtual bool IsStruct() const {return false;}
    virtual bool IsResource() const {return false;}

    uint32 GetBytes() const {return bytes;}

protected:
    uint32 bytes = 0;
};

class ProgramData
{

};

class ProgramReflection
{
public:
    struct BindingData
    {
        int32 slot = -1;  
        String name;
        DescriptorType descriptorType = DescriptorType::Unknown;
        uint32 binding = 0;
        uint32 arrayLength = 1;
        uint32 set = 0;
    };

    struct SetData
    {
        Array<int32> bindingSlots;
        ShaderVisibilityFlags visibility = ShaderVisibility::All;
    };

    ProgramReflection() = default;

    const RootSignatureDesc &GetRootSignatureDesc() const;

    void AddBindingData(const String &name, DescriptorType descriptorType, uint32 binding, uint32 arrayLength = 1, uint32 setIndex = 0)
    {
        CT_CHECK(dirty);

        bindingDatas.Add({bindingDatas.Count(), name, descriptorType, binding, arrayLength, setIndex});
        const int32 bindingDataIndex = bindingDatas.Count() - 1;

        if (setDatas.Count() <= setIndex)
        {
            setDatas.SetCount(setIndex + 1);
        }

        if (setDatas[setIndex].bindingSlots.Count() <= binding)
        {
            setDatas[setIndex].bindingSlots.Add(-1, binding - setDatas[setIndex].bindingSlots.Count() + 1);
        }
        setDatas[setIndex].bindingSlots[binding] = bindingDataIndex;
        bindingNames.Put(name, bindingDataIndex);
    }

    const BindingData &GeteBindingData(int32 slot) const
    {
        static const BindingData INVALID = {};

        if (slot < 0 || slot >= bindingDatas.Count())
            return INVALID;
        return bindingDatas[slot];
    }

    const BindingData &GetBindingData(const String &name) const
    {
        auto *ptr = bindingNames.TryGet(name);
        int32 slot = ptr ? *ptr : -1;
        return GetBindingData(slot);
    }

    uint32 GetDescriptorSetCount() const
    {
        return setDatas.Count();
    }

    const Array<int32> &GetBindingSlots(uint32 setIndex) const
    {
        return setDatas[setIndex].bindingSlots;
    }

    const SPtr<DescriptorSetLayout> &GetDescriptorSetLayout(uint32 setIndex) const
    {
        return GetRootSignatureDesc().layouts[setIndex];
    }

    static SPtr<ProgramReflection> Create()
    {
        return Memory::MakeShared<ProgramReflection>();
    }

private:
    mutable RootSignatureDesc rootSignatureDesc;
    mutable bool dirty = true;

    Array<BindingData> bindingDatas;
    Array<SetData> setDatas;
    HashMap<String, int32> bindingNames;
};


}
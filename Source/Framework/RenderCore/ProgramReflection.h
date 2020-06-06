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

struct ShaderVarLocation
{
    uint32 binding = 0;
    uint32 arrayIndex = 0;
    uint32 byteOffset = 0;
};

class ReflectionDataType;
class ReflectionResourceType;
class ReflectionArrayType;
class ReflectionStructType;
class ReflectionVar;
class ParameterBlockReflection;

class ReflectionType
{
public:
    struct Range
    {

    };

    ReflectionType(uint32 size) : size(size)
    {
    }

    virtual ~ReflectionType() = default;

    virtual bool IsArray() const { return false; }
    virtual bool IsStruct() const { return false; }
    virtual bool IsData() const { return false; }
    virtual bool IsResource() const { return false; }

    virtual uint32 GetSize() const { return size; }

    const ReflectionDataType *AsData() const;
    const ReflectionResourceType *AsResource() const;
    const ReflectionArrayType *AsArray() const;
    const ReflectionStructType *AsStruct() const;
    const ReflectionType *UnwrapArray() const;
    uint32 GetTotalElementCount() const;
    SPtr<ReflectionVar> FindMember(const String &name) const;

protected:
    uint32 size = 0;
};

class ReflectionDataType : public ReflectionType
{
public:
    ReflectionDataType(ShaderDataType dataType, uint32 size)
        : ReflectionType(size), dataType(dataType)
    {
    }

    virtual bool IsData() const override 
    { 
        return true; 
    }

    ShaderDataType GetShaderDataType() const
    {
        return dataType;
    }

    static SPtr<ReflectionDataType> Create(ShaderDataType dataType, uint32 size)
    {
        return Memory::MakeShared<ReflectionDataType>(dataType, size);
    }

private:
    ShaderDataType dataType = ShaderDataType::Unknown;
};

class ReflectionResourceType : public ReflectionType
{
public:
    ReflectionResourceType(ShaderResourceType resourceType, ShaderAccess shaderAccess)
        : ReflectionType(0), resourceType(resourceType), shaderAccess(shaderAccess)
    {
        //TODO Range
    }

    virtual bool IsResource() const override
    {
        return true;
    }

    virtual uint32 GetSize() const override
    {
        if(structType)
            return structType->GetSize();
        return 0;
    }

    ShaderResourceType GetShaderResourceType() const
    {
        return resourceType;
    }

    ShaderAccess GetShaderAccess() const
    {
        return shaderAccess;
    }

    const SPtr<ReflectionType> &GetStructType() const
    {
        return structType;
    }

    template <typename... Args>
    static SPtr<ReflectionResourceType> Create(Args&&... args)
    {
        return Memory::MakeShared<ReflectionResourceType>(std::forward<Args>(args)...);
    }

private:
    ShaderResourceType resourceType = ShaderResourceType::Unknown;
    ShaderAccess shaderAccess = ShaderAccess::Undefined;
    SPtr<ReflectionType> structType;
};

class ReflectionArrayType : public ReflectionType
{
public:
    ReflectionArrayType(uint32 elementCount, uint32 stride, const SPtr<ReflectionType> &elementType, uint32 totalSize)
        : ReflectionType(totalSize), elementCount(elementCount), stride(stride), elementType(elementType)
    {
        //TODO Range
    }

    virtual bool IsArray() const override
    { 
        return true; 
    }

    uint32 GetElementCount() const
    {
        return elementCount;
    }

    const SPtr<ReflectionType> &GetElementType() const
    {
        return elementType;
    }

    template <typename... Args>
    static SPtr<ReflectionArrayType> Create(Args&&... args)
    {
        return Memory::MakeShared<ReflectionArrayType>(std::forward<Args>(args)...);
    }

private:
    uint32 elementCount = 0;
    uint32 stride = 0;
    SPtr<ReflectionType> elementType;
};

class ReflectionStructType : public ReflectionType
{
public:
    ReflectionStructType(uint32 size, const String &name)
        : ReflectionType(size), name(name)
    {
    }

    virtual bool IsStruct() const override
    {
        return true;
    }

    const String &GetName() const
    {
        return name;
    }

    uint32 GetMemberCount() const
    {
        return members.Count();
    }

    CT_INLINE int32 AddMember(const SPtr<ReflectionVar> &var);

    SPtr<ReflectionVar> GetMember(int32 index) const;
    SPtr<ReflectionVar> GetMember(const String &name) const;
    int32 GetMemberIndex(const String &name) const;

    static SPtr<ReflectionStructType> Create(uint32 size, const String &name)
    {
        return Memory::MakeShared<ReflectionStructType>(size, name);
    }
    
private:
    String name;
    Array<SPtr<ReflectionVar>> members;
    HashMap<String, int32> memberNames;

    uint32 cbvCount = 0;
    uint32 srvCount = 0;
    uint32 uavCount = 0;
    uint32 samplerCount = 0;
};

class ReflectionVar
{
public:
    ReflectionVar(const String &name, const SPtr<ReflectionType> &reflectionType, const ShaderVarLocation location)
        : name(name), reflectionType(reflectionType), location(location)
    {
    }

    const String &GetName() const
    {
        return name;
    }

    const SPtr<ReflectionType> &GetReflectionType() const
    {
        return reflectionType;
    };

    ShaderVarLocation GetLocation() const
    {
        return location;
    }

    uint32 GetByteOffset() const
    {
        return location.byteOffset;
    }

    static SPtr<ReflectionVar> Create(const String &name, const SPtr<ReflectionType> &reflectionType, const ShaderVarLocation location)
    {
        return Memory::MakeShared<ReflectionVar>(name, reflectionType, location);
    }

private:
    String name;
    SPtr<ReflectionType> reflectionType;
    ShaderVarLocation location;
};

class ParameterBlockReflection
{
public:
    void Finalize();

    //SPtr<ReflectionVar> GetMember(const String &name);

    static SPtr<ParameterBlockReflection> Create()
    {
        return Memory::MakeShared<ParameterBlockReflection>();
    }

private:
    friend class ProgramReflectionBuilder;

    SPtr<ReflectionType> elementType;
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

    void Finalize();

    SPtr<ParameterBlockReflection> GetDefaultParameterBlock() const
    {
        return defaultBlock;
    }

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
    friend class ProgramReflectionBuilder;

    mutable RootSignatureDesc rootSignatureDesc;
    mutable bool dirty = true;

    SPtr<ParameterBlockReflection> defaultBlock;

    Array<BindingData> bindingDatas;
    Array<SetData> setDatas;
    HashMap<String, int32> bindingNames;
};

//========================================================================

CT_INLINE int32 ReflectionStructType::AddMember(const SPtr<ReflectionVar> &var)
{
    if (memberNames.Contains(var->GetName()))
    {
        CT_LOG(Error, CT_TEXT("Struct type add member failed, member named {0} already exists."), var->GetName());
        return -1;
    }

    int32 newIndex = members.Count();
    members.Add(var);
    memberNames.Put(var->GetName(), newIndex);

    auto memberType = var->GetReflectionType();
    //TODO

    return newIndex;
}

class ProgramReflectionBuilder
{
public:
    ProgramReflectionBuilder()
    {
        auto globalStruct = ReflectionStructType::Create(0, CT_TEXT(""));
        auto blockReflection = ParameterBlockReflection::Create();
        blockReflection->elementType = globalStruct;

        reflection = ProgramReflection::Create();
        reflection->defaultBlock = blockReflection;
    }

    void Build()
    {
        reflection->Finalize();
    }

    const SPtr<ProgramReflection> &GetReflection() const
    {
        return reflection;
    }

private:
    SPtr<ProgramReflection> reflection;
};

}
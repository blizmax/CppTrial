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
    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
    Texture2DMS,
    Texture1DArray,
    Texture2DArray,
    TextureCubeArray,
    Texture2DMSArray,
    Sampler,
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
    uint32 rangeIndex = 0;
    uint32 arrayIndex = 0;
    uint32 byteOffset = 0;
};

struct BindingRange
{
    DescriptorType descriptorType = DescriptorType::Unknown;
    uint32 count = 1;
    uint32 baseIndex = 0;
};

struct BindingInfo
{
    String name;
    uint32 binding = 0;
    uint32 set = 0;
};

struct SetInfo
{
    uint32 set = 0;
    Array<int32> bindingIndices;
    SPtr<DescriptorSetLayout> layout;

    SetInfo() = default;
    SetInfo(uint32 set) : set(set)
    {
    }
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
    virtual ~ReflectionType() = default;

    virtual bool IsArray() const { return false; }
    virtual bool IsStruct() const { return false; }
    virtual bool IsData() const { return false; }
    virtual bool IsResource() const { return false; }

    virtual String ToString() const = 0;

    virtual uint32 GetSize() const { return size; }
    void SetSize(uint32 newSize) { size = newSize; }

    const ReflectionDataType *AsData() const;
    const ReflectionResourceType *AsResource() const;
    const ReflectionArrayType *AsArray() const;
    const ReflectionStructType *AsStruct() const;
    const ReflectionType *UnwrapArray() const;
    uint32 GetTotalElementCount() const;
    SPtr<ReflectionVar> FindMember(const String &name) const;

    uint32 GetBindingRangeCount() const
    {
        return bindingRanges.Count();
    }

    const BindingRange &GetBindingRange(int32 index) const
    {
        return bindingRanges[index];
    }

    const Array<BindingRange> &GetBindingRanges() const
    {
        return bindingRanges;
    }

protected:
    uint32 size = 0;
    Array<BindingRange> bindingRanges;
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

    String ToString() const
    {
        return String::Format(CT_TEXT("{0}: {1}"), name, reflectionType ? reflectionType->ToString() : CT_TEXT(""));
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

class ReflectionDataType : public ReflectionType
{
public:
    ReflectionDataType(ShaderDataType dataType) : dataType(dataType)
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

    virtual String ToString() const override
    {
        return String::Format(CT_TEXT("DataType"));
    }

    static SPtr<ReflectionDataType> Create(ShaderDataType dataType)
    {
        return Memory::MakeShared<ReflectionDataType>(dataType);
    }

private:
    ShaderDataType dataType = ShaderDataType::Unknown;
};

class ReflectionResourceType : public ReflectionType
{
public:
    ReflectionResourceType(ShaderResourceType resourceType, ShaderAccess shaderAccess)
        : resourceType(resourceType), shaderAccess(shaderAccess)
    {
        DescriptorType descriptorType = DescriptorType::Unknown;

        switch (resourceType)
        {
        case ShaderResourceType::ConstantBuffer:
            descriptorType = DescriptorType::Cbv;
            break;
        case ShaderResourceType::RawBuffer:
            descriptorType =
                shaderAccess == ShaderAccess::Read ? DescriptorType::RawBufferSrv : DescriptorType::RawBufferUav;
            break;
        case ShaderResourceType::StructuredBuffer:
            descriptorType =
                shaderAccess == ShaderAccess::Read ? DescriptorType::StructuredBufferSrv : DescriptorType::StructuredBufferUav;
            break;
        case ShaderResourceType::TypedBuffer:
            descriptorType =
                shaderAccess == ShaderAccess::Read ? DescriptorType::TypedBufferSrv : DescriptorType::TypedBufferUav;
            break;
        case ShaderResourceType::Sampler:
            descriptorType = DescriptorType::Sampler;
            break;
        case ShaderResourceType::Texture1D:
        case ShaderResourceType::Texture2D:
        case ShaderResourceType::Texture3D:
        case ShaderResourceType::TextureCube:
        case ShaderResourceType::Texture2DMS:
        case ShaderResourceType::Texture1DArray:
        case ShaderResourceType::Texture2DArray:
        case ShaderResourceType::TextureCubeArray:
        case ShaderResourceType::Texture2DMSArray:
            descriptorType =
                shaderAccess == ShaderAccess::Read ? DescriptorType::TextureSrv : DescriptorType::TextureUav;
            break;
        }

        BindingRange range;
        range.descriptorType = descriptorType;
        range.count = 1;
        range.baseIndex = 0;
        bindingRanges.Add(range);
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

    virtual String ToString() const override
    {
        String ret = CT_TEXT("ResourceType");
        if(structType)
        {
            ret += String::Format(CT_TEXT(", inner {0}"), structType->ToString());
        }
        return ret;
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

    void SetStructType(const SPtr<ReflectionType> &newType)
    {
        structType = newType;
    }

    const SPtr<ParameterBlockReflection> &GetBlockReflection() const
    {
        return blockReflection;
    }

    void SetBlockReflection(const SPtr<ParameterBlockReflection> &reflection)
    {
        blockReflection = reflection;
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
    SPtr<ParameterBlockReflection> blockReflection;
};

class ReflectionArrayType : public ReflectionType
{
public:
    ReflectionArrayType(uint32 elementCount, uint32 stride, const SPtr<ReflectionType> &elementType)
        : elementCount(elementCount), stride(stride), elementType(elementType)
    {
        for (auto &e : elementType->GetBindingRanges())
        {
            BindingRange range;
            range.descriptorType = e.descriptorType;
            range.count = e.count * elementCount;
            range.baseIndex = e.baseIndex * elementCount;

            bindingRanges.Add(range);
        }
    }

    virtual bool IsArray() const override
    { 
        return true; 
    }

    virtual String ToString() const override
    {
        return String::Format(CT_TEXT("ArrayType, {0}elements of ({1})"), elementCount, elementType->ToString());
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
    ReflectionStructType(const String &name) : name(name)
    {
    }

    virtual bool IsStruct() const override
    {
        return true;
    }

    virtual String ToString() const override
    {
        String ret = CT_TEXT("StructType, {\n");
        bool first = true;
        for(const auto &m : members)
        {
            if(!first) ret += CT_TEXT("\n");
            ret += m->ToString();
            first = false;
        }
        ret += CT_TEXT("}");
        return ret;
    }

    const String &GetName() const
    {
        return name;
    }

    uint32 GetMemberCount() const
    {
        return members.Count();
    }

    int32 AddMember(const SPtr<ReflectionVar> &var);

    SPtr<ReflectionVar> GetMember(int32 index) const;
    SPtr<ReflectionVar> GetMember(const String &name) const;
    int32 GetMemberIndex(const String &name) const;

    static SPtr<ReflectionStructType> Create(const String &name)
    {
        return Memory::MakeShared<ReflectionStructType>(name);
    }
    
private:
    String name;
    Array<SPtr<ReflectionVar>> members;
    HashMap<String, int32> memberNames;
};

class ParameterBlockReflection
{
public:
    void Finalize();

    //SPtr<ReflectionVar> GetMember(const String &name);

    const SPtr<ReflectionType> &GetElementType() const
    {
        return elementType;
    }

    void SetElementType(const SPtr<ReflectionType> &newType)
    {
        elementType = newType;
    }

    uint32 GetBindingInfoCount() const
    {
        return bindingInfos.Count();
    }

    const BindingInfo &GetBindingInfo(int32 index) const
    {
        return bindingInfos[index];
    }

    const Array<BindingInfo> &GetBindingInfos() const
    {
        return bindingInfos;
    }

    void AddBindingInfo(const BindingInfo &info)
    {
        bindingInfos.Add(info);
    }

    uint32 GetSetInfoCount() const
    {
        return setInfos.Count();
    }

    const SetInfo &GetSetInfo(int32 setIndex) const
    {
        return setInfos[setIndex];
    }

    const Array<SetInfo> &GetSetInfos() const
    {
        return setInfos;
    }

    const SPtr<DescriptorSetLayout> &GetDescriptorSetLayout(uint32 setIndex) const
    {
        return setInfos[setIndex].layout;
    }

    static SPtr<ParameterBlockReflection> Create()
    {
        return Memory::MakeShared<ParameterBlockReflection>();
    }

private:
    friend class ProgramReflection;
    friend class ParameterBlock;

    SPtr<ReflectionType> elementType;
    Array<BindingInfo> bindingInfos;
    Array<SetInfo> setInfos;

    int32 cbvCount = 0;
    int32 srvCount = 0;
    int32 uavCount = 0;
    int32 samplerCount = 0;
};

class ProgramReflection
{
public:
    ProgramReflection() = default;

    void Finalize();

    SPtr<ParameterBlockReflection> GetDefaultBlockReflection() const
    {
        return defaultBlockReflection;
    }

    void SetDefaultBlockReflection(const SPtr<ParameterBlockReflection> &reflection)
    {
        defaultBlockReflection = reflection;
    }

    const RootSignatureDesc &GetRootSignatureDesc() const
    {
        CT_CHECK(finalized);
        return rootSignatureDesc;
    }

    static SPtr<ProgramReflection> Create()
    {
        return Memory::MakeShared<ProgramReflection>();
    }

private:
    RootSignatureDesc rootSignatureDesc;
    bool finalized = false;
    SPtr<ParameterBlockReflection> defaultBlockReflection;
};


class ProgramReflectionBuilder
{
public:
    ProgramReflectionBuilder()
    {
        auto globalStruct = ReflectionStructType::Create(CT_TEXT(""));
        auto blockReflection = ParameterBlockReflection::Create();
        blockReflection->SetElementType(globalStruct);

        reflection = ProgramReflection::Create();
        reflection->SetDefaultBlockReflection(blockReflection);
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

//========================================================================
//#define CT_RENDER_CORE_PROGRAM_REFLECTION_IMPLEMENT
#ifdef CT_RENDER_CORE_PROGRAM_REFLECTION_IMPLEMENT
int32 ReflectionStructType::AddMember(const SPtr<ReflectionVar> &var)
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
    for (auto &e : memberType->GetBindingRanges())
    {
        BindingRange range;
        range.descriptorType = e.descriptorType;
        range.count = e.count;
        range.baseIndex = e.baseIndex;

        bindingRanges.Add(range);
    }

    return newIndex;
}

void ParameterBlockReflection::Finalize()
{
    uint32 maxSetIndex = 0;

    const auto &bindingRanges = elementType->GetBindingRanges();
    CT_CHECK(bindingInfos.Count() == bindingRanges.Count());

    HashMap<uint32, SetInfo> setInfoMap;
    for(int32 i = 0; i < bindingInfos.Count(); ++i)
    {
        const auto &info = bindingInfos[i];
        uint32 set = info.set;
        if (maxSetIndex < set) maxSetIndex = set;
        if (!setInfoMap.Contains(set))
            setInfoMap.Put(set, {set});
        setInfoMap[set].bindingIndices.Add(i);
    }

    for(uint32 s = 0; s <= maxSetIndex; ++s)
    {
        setInfos.Add({s});
    }
    if(setInfos.Count() < setInfoMap.Count())
    {
        CT_LOG(Warning, CT_TEXT("ParameterBlockReflection setInfos has hole."));
    }
    for(auto &[set, info] : setInfoMap)
    {
        setInfos[set] = std::move(info);
    }

    for (auto &e : elementType->GetBindingRanges())
    {
        switch (e.descriptorType)
        {
        case DescriptorType::Cbv:
            cbvCount++;
            break;
        case DescriptorType::TextureSrv:
        case DescriptorType::RawBufferSrv:
        case DescriptorType::TypedBufferSrv:
        case DescriptorType::StructuredBufferSrv:
            srvCount++;
            break;
        case DescriptorType::TextureUav:
        case DescriptorType::RawBufferUav:
        case DescriptorType::TypedBufferUav:
        case DescriptorType::StructuredBufferUav:
            uavCount++;
            break;
        case DescriptorType::Sampler:
            samplerCount++;
            break;
        }
    }
}

#endif

}
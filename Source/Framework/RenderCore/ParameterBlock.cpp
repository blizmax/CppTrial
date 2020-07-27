#include "RenderCore/ParameterBlock.h"
#include "RenderCore/RenderAPI.h"

#define LOG_WRN_RESOURCE_LOCATION(f, t) CT_LOG(Warning, CT_TEXT("ParameterBlock {0} {1} out of index."), CT_TEXT(#f), CT_TEXT(#t))
#define LOG_WRN_DESCRIPTOR_TYPE(f, t) CT_LOG(Warning, CT_TEXT("ParameterBlock {0} {1} mismatch descriptor type."), CT_TEXT(#f), CT_TEXT(#t))

SPtr<ParameterBlock> ParameterBlock::Create(const SPtr<ParameterBlockReflection> &reflection)
{
    return Memory::MakeShared<ParameterBlock>(reflection);
}

ParameterBlock::ParameterBlock(const SPtr<ParameterBlockReflection> &reflection)
    : reflection(reflection)
{
    sets.SetCount(reflection->GetSetInfoCount());
    constBufferData.SetCount(reflection->GetElementType()->GetSize());

    auto structType = reflection->GetElementType()->AsStruct();
    srvs.SetCount(structType->srvCount);
    uavs.SetCount(structType->uavCount);
    samplers.SetCount(structType->samplerCount);
    parameterBlocks.SetCount(structType->cbvCount);

    CreateParameterBlocks(GetRootVar());
}

void ParameterBlock::CreateParameterBlocks(const ShaderVar &var)
{
    auto varType = var.GetVarType();

    if (auto resourceType = varType->AsResource())
    {
        if (resourceType->GetShaderResourceType() == ShaderResourceType::ConstantBuffer)
        {
            auto block = ParameterBlock::Create(resourceType->GetBlockReflection());
            var.SetParameterBlock(block);
        }
    }
    else if (auto structType = varType->AsStruct())
    {
        for (int32 i = 0; i < structType->GetMemberCount(); ++i)
        {
            CreateParameterBlocks(var.FindMember(i));
        }
    }
}

int32 ParameterBlock::GetFlatIndex(const ShaderVarLocation &location) const
{
    auto &elementType = GetElementType();
    auto &range = elementType->GetBindingRange(location.rangeIndex);
    return range.baseIndex + location.arrayIndex;
}

bool ParameterBlock::CheckResourceLocation(const ShaderVarLocation &location) const
{
    if (!location.IsValid())
        return false;
    auto &elementType = GetElementType();
    if (location.rangeIndex >= elementType->GetBindingRangeCount())
        return false;
    auto &range = elementType->GetBindingRange(location.rangeIndex);
    if (location.arrayIndex >= range.count)
        return false;
    return true;
}

bool ParameterBlock::CheckDescriptorType(const ShaderVarLocation &location, DescriptorType descriptorType) const
{
    auto &elementType = GetElementType();
    auto &range = elementType->GetBindingRange(location.rangeIndex);
    if (range.descriptorType == descriptorType)
        return true;
    return false;
}

bool ParameterBlock::CheckDescriptorSrv(const ShaderVarLocation &location, const ResourceView *view) const
{
    auto &elementType = GetElementType();
    auto &range = elementType->GetBindingRange(location.rangeIndex);
    auto descriptorType = range.descriptorType;

    if (descriptorType == DescriptorType::TextureSrv || descriptorType == DescriptorType::RawBufferSrv || descriptorType == DescriptorType::TypedBufferSrv || descriptorType == DescriptorType::StructuredBufferSrv)
        return true;
    return false;
}

bool ParameterBlock::CheckDescriptorUav(const ShaderVarLocation &location, const ResourceView *view) const
{
    auto &elementType = GetElementType();
    auto &range = elementType->GetBindingRange(location.rangeIndex);
    auto descriptorType = range.descriptorType;

    if (descriptorType == DescriptorType::TextureUav || descriptorType == DescriptorType::RawBufferUav || descriptorType == DescriptorType::TypedBufferUav || descriptorType == DescriptorType::StructuredBufferUav)
        return true;
    return false;
}

static const ReflectionResourceType *GetResourceType(const ShaderVar &var, const Resource *resource)
{
    if (!var.IsValid())
        return nullptr;

    auto resourceType = var.GetVarType()->UnwrapArray()->AsResource();
    if (!resourceType)
        return nullptr;

    if (resource)
    {
        ResourceBindFlags bindFlags = 0;
        switch (resourceType->GetShaderAccess())
        {
        case ShaderAccess::Read:
            bindFlags = (resourceType->GetShaderResourceType() == ShaderResourceType::ConstantBuffer) ? ResourceBind::Constant : ResourceBind::ShaderResource;
            break;
        case ShaderAccess::ReadWrite:
            bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess; //ResourceBind::UnorderedAccess;
            break;
        default:
            CT_EXCEPTION(RenderCore, "Invalid shader access.");
            return nullptr;
        }
        if ((resource->GetBindFlags() & bindFlags) == 0)
        {
            CT_EXCEPTION(RenderCore, "Resource bind flag error.");
            return nullptr;
        }
    }

    return resourceType;
}

bool ParameterBlock::IsBufferVarValid(const ShaderVar &var, const Buffer *buffer) const
{
    auto resourceType = GetResourceType(var, buffer);
    if (!resourceType)
        return false;

    return true;

    // TODO
    // switch (resourceType->GetShaderResourceType())
    // {
    // case ShaderResourceType::RawBuffer:
    // case ShaderResourceType::ConstantBuffer:
    //     return true;
    // case ShaderResourceType::StructuredBuffer:
    //     if (buffer && !buffer->IsStructured())
    //     {
    //         CT_EXCEPTION(RenderCore, "Structured buffer required.");
    //         return false;
    //     }
    //     return true;
    // case ShaderResourceType::TypedBuffer:
    //     if (buffer && !buffer->IsTyped())
    //     {
    //         CT_EXCEPTION(RenderCore, "Typed buffer required.");
    //         return false;
    //     }
    //     return true;
    // default:
    //     CT_EXCEPTION(RenderCore, "Shader var is not a buffer var.");
    //     return false;
    // }
}

bool ParameterBlock::IsTextureVarValid(const ShaderVar &var, const Texture *texture) const
{
    auto resourceType = GetResourceType(var, texture);
    if (!resourceType)
        return false;

    switch (resourceType->GetShaderResourceType())
    {
    case ShaderResourceType::Texture1D:
    case ShaderResourceType::Texture2D:
    case ShaderResourceType::Texture3D:
    case ShaderResourceType::TextureCube:
    case ShaderResourceType::Texture2DMS:
    case ShaderResourceType::Texture1DArray:
    case ShaderResourceType::Texture2DArray:
    case ShaderResourceType::TextureCubeArray:
    case ShaderResourceType::Texture2DMSArray:
        return true;
    default:
        CT_EXCEPTION(RenderCore, "Shader var is not a texture var.");
        return false;
    }
}

bool ParameterBlock::IsSamplerVarValid(const ShaderVar &var, const Sampler *sampler) const
{
    auto resourceType = GetResourceType(var, nullptr);
    if (!resourceType)
        return false;

    if (resourceType->GetShaderResourceType() == ShaderResourceType::Sampler)
        return true;

    CT_EXCEPTION(RenderCore, "Shader var is not a sampler var.");
    return false;
}

bool ParameterBlock::IsParameterBlockVarValid(const ShaderVar &var, const ParameterBlock *block) const
{
    auto resourceType = GetResourceType(var, nullptr);
    if (!resourceType)
        return false;

    if (resourceType->GetShaderResourceType() == ShaderResourceType::ConstantBuffer)
        return true;

    CT_EXCEPTION(RenderCore, "Shader var is not a parameter block var.");
    return false;
}

void ParameterBlock::MarkDescriptorSetDirty(const ShaderVarLocation &location)
{
    auto setIndex = reflection->GetBindingInfo(location.rangeIndex).set;
    MarkDescriptorSetDirty(setIndex);
}

void ParameterBlock::MarkDescriptorSetDirty(int32 setIndex)
{
    if (parent)
    {
        parent->MarkDescriptorSetDirty(setIndex);
    }
    else
    {
        sets[setIndex] = nullptr;
    }
}

void ParameterBlock::MarkUniformDataDirty()
{
    constantBufferDirty = true;
    MarkDescriptorSetDirty(reflection->GetConstantBufferBindingInfo().set);
}

static bool IsUavOrSrv(const ReflectionResourceType *resourceType, bool &isUav)
{
    if (!resourceType)
        return false;
    if (resourceType->GetShaderResourceType() == ShaderResourceType::Sampler ||
        resourceType->GetShaderResourceType() == ShaderResourceType::ConstantBuffer)
        return false;

    CT_CHECK(resourceType->GetShaderAccess() != ShaderAccess::Undefined);
    isUav = (resourceType->GetShaderAccess() == ShaderAccess::ReadWrite);
    return true;
}

Resource *ParameterBlock::GetResourceSrvUavCommon(const ShaderVarLocation &location, bool isTexture) const
{
    if (!CheckResourceLocation(location))
    {
        if (isTexture)
            LOG_WRN_RESOURCE_LOCATION(get, texture);
        else
            LOG_WRN_RESOURCE_LOCATION(get, buffer);
        return nullptr;
    }

    auto resourceType = location.varType->UnwrapArray()->AsResource();
    bool isUav;
    if (IsUavOrSrv(resourceType, isUav))
    {
        auto flatIndex = GetFlatIndex(location);
        if (isUav)
        {
            if (CheckDescriptorUav(location, nullptr))
            {
                return uavs[flatIndex] ? uavs[flatIndex]->GetResource() : nullptr;
            }
        }
        else
        {
            if (CheckDescriptorSrv(location, nullptr))
            {
                return srvs[flatIndex] ? srvs[flatIndex]->GetResource() : nullptr;
            }
        }
    }

    if (isTexture)
        LOG_WRN_DESCRIPTOR_TYPE(get, texture);
    else
        LOG_WRN_DESCRIPTOR_TYPE(get, buffer);
    return nullptr;
}

bool ParameterBlock::SetResourceSrvUavCommon(const ShaderVarLocation &location, const SPtr<Resource> &resource, bool isTexture)
{
    if (!CheckResourceLocation(location))
    {
        if (isTexture)
            LOG_WRN_RESOURCE_LOCATION(set, texture);
        else
            LOG_WRN_RESOURCE_LOCATION(set, buffer);
        return false;
    }

    auto resourceType = location.varType->UnwrapArray()->AsResource();
    bool isUav;
    if (IsUavOrSrv(resourceType, isUav))
    {
        auto flatIndex = GetFlatIndex(location);
        if (isUav)
        {
            auto uav = resource->GetUav();
            if (!uav)
                return false;

            if (CheckDescriptorUav(location, uav.get()))
            {
                if (uavs[flatIndex] != uav)
                {
                    uavs[flatIndex] = uav;
                    MarkDescriptorSetDirty(location);
                }
                return true;
            }
        }
        else
        {
            auto srv = resource->GetSrv();
            if (!srv)
                return false;

            if (CheckDescriptorSrv(location, srv.get()))
            {
                if (srvs[flatIndex] != srv)
                {
                    srvs[flatIndex] = srv;
                    MarkDescriptorSetDirty(location);
                }
                return true;
            }
        }
    }

    if (isTexture)
        LOG_WRN_DESCRIPTOR_TYPE(set, texture);
    else
        LOG_WRN_DESCRIPTOR_TYPE(set, buffer);
    return false;
}

ShaderVar ParameterBlock::GetRootVar() const
{
    return ShaderVar(const_cast<ParameterBlock *>(this));
}

SPtr<Buffer> ParameterBlock::GetUnderlyingConstantBuffer() const
{
    if (constantBuffer == nullptr)
    {
        uint32 size = GetElementType()->GetSize();
        constantBuffer = Buffer::Create(size, ResourceBind::Constant, BufferCpuAccess::Write);
    }
    return constantBuffer;
}

SPtr<Buffer> ParameterBlock::GetBuffer(const ShaderVarLocation &location) const
{
    return GetResourceSrvUavCommon(location, false)->AsBuffer();
}

SPtr<Buffer> ParameterBlock::GetBuffer(const String &name) const
{
    auto var = GetRootVar()[name];
    if (!IsBufferVarValid(var, nullptr))
        return nullptr;
    return var.GetBuffer();
}

SPtr<Texture> ParameterBlock::GetTexture(const ShaderVarLocation &location) const
{
    return GetResourceSrvUavCommon(location, true)->AsTexture();
}

SPtr<Texture> ParameterBlock::GetTexture(const String &name) const
{
    auto var = GetRootVar()[name];
    if (!IsTextureVarValid(var, nullptr))
        return nullptr;
    return var.GetTexture();
}

SPtr<ResourceView> ParameterBlock::GetSrv(const ShaderVarLocation &location) const
{
    if (!CheckResourceLocation(location))
    {
        LOG_WRN_RESOURCE_LOCATION(get, srv);
        return nullptr;
    }

    if (!CheckDescriptorSrv(location, nullptr))
    {
        LOG_WRN_DESCRIPTOR_TYPE(get, srv);
        return nullptr;
    }

    auto flatIndex = GetFlatIndex(location);
    return srvs[flatIndex];
}

SPtr<ResourceView> ParameterBlock::GetUav(const ShaderVarLocation &location) const
{
    if (!CheckResourceLocation(location))
    {
        LOG_WRN_RESOURCE_LOCATION(get, uav);
        return nullptr;
    }

    if (!CheckDescriptorUav(location, nullptr))
    {
        LOG_WRN_DESCRIPTOR_TYPE(get, uav);
        return nullptr;
    }

    auto flatIndex = GetFlatIndex(location);
    return uavs[flatIndex];
}

SPtr<Sampler> ParameterBlock::GetSampler(const ShaderVarLocation &location) const
{
    if (!CheckResourceLocation(location))
    {
        LOG_WRN_RESOURCE_LOCATION(get, sampler);
        return nullptr;
    }

    if (!CheckDescriptorType(location, DescriptorType::Sampler))
    {
        LOG_WRN_DESCRIPTOR_TYPE(get, sampler);
        return nullptr;
    }

    auto flatIndex = GetFlatIndex(location);
    return samplers[flatIndex];
}

SPtr<Sampler> ParameterBlock::GetSampler(const String &name) const
{
    auto var = GetRootVar()[name];
    if (!IsSamplerVarValid(var, nullptr))
        return nullptr;
    return var.GetSampler();
}

SPtr<ParameterBlock> ParameterBlock::GetParameterBlock(const ShaderVarLocation &location) const
{
    if (!CheckResourceLocation(location))
    {
        LOG_WRN_RESOURCE_LOCATION(get, parameter block);
        return nullptr;
    }

    if (!CheckDescriptorType(location, DescriptorType::Cbv))
    {
        LOG_WRN_DESCRIPTOR_TYPE(get, parameter block);
        return nullptr;
    }

    auto flatIndex = GetFlatIndex(location);
    return parameterBlocks[flatIndex];
}

SPtr<ParameterBlock> ParameterBlock::GetParameterBlock(const String &name) const
{
    auto var = GetRootVar()[name];
    if (!IsParameterBlockVarValid(var, nullptr))
        return nullptr;
    return var.GetParameterBlock();
}

bool ParameterBlock::SetBuffer(const ShaderVarLocation &location, const SPtr<Buffer> &buffer)
{
    return SetResourceSrvUavCommon(location, buffer, false);
}

bool ParameterBlock::SetBuffer(const String &name, const SPtr<Buffer> &buffer)
{
    auto var = GetRootVar()[name];
    if (!IsBufferVarValid(var, buffer.get()))
        return false;
    return var.SetBuffer(buffer);
}

bool ParameterBlock::SetTexture(const ShaderVarLocation &location, const SPtr<Texture> &texture)
{
    return SetResourceSrvUavCommon(location, texture, true);
}

bool ParameterBlock::SetTexture(const String &name, const SPtr<Texture> &texture)
{
    auto var = GetRootVar()[name];
    if (!IsTextureVarValid(var, texture.get()))
        return false;
    return var.SetTexture(texture);
}

bool ParameterBlock::SetSrv(const ShaderVarLocation &location, const SPtr<ResourceView> &srv)
{
    if (!CheckResourceLocation(location))
    {
        LOG_WRN_RESOURCE_LOCATION(set, srv);
        return false;
    }

    if (!CheckDescriptorSrv(location, srv.get()))
    {
        LOG_WRN_DESCRIPTOR_TYPE(set, srv);
        return false;
    }

    auto flatIndex = GetFlatIndex(location);
    if (srvs[flatIndex] == srv)
        return true;
    srvs[flatIndex] = srv;
    MarkDescriptorSetDirty(location);
    return true;
}

bool ParameterBlock::SetUav(const ShaderVarLocation &location, const SPtr<ResourceView> &uav)
{
    if (!CheckResourceLocation(location))
    {
        LOG_WRN_RESOURCE_LOCATION(set, uav);
        return false;
    }

    if (!CheckDescriptorUav(location, uav.get()))
    {
        LOG_WRN_DESCRIPTOR_TYPE(set, uav);
        return false;
    }

    auto flatIndex = GetFlatIndex(location);
    if (uavs[flatIndex] == uav)
        return true;
    uavs[flatIndex] = uav;
    MarkDescriptorSetDirty(location);
    return true;
}

bool ParameterBlock::SetSampler(const ShaderVarLocation &location, const SPtr<Sampler> &sampler)
{
    if (!CheckResourceLocation(location))
    {
        LOG_WRN_RESOURCE_LOCATION(set, sampler);
        return false;
    }

    if (!CheckDescriptorType(location, DescriptorType::Sampler))
    {
        LOG_WRN_DESCRIPTOR_TYPE(set, sampler);
        return false;
    }

    auto flatIndex = GetFlatIndex(location);
    if (samplers[flatIndex] == sampler)
        return true;
    samplers[flatIndex] = sampler;
    MarkDescriptorSetDirty(location);
    return true;
}

bool ParameterBlock::SetSampler(const String &name, const SPtr<Sampler> &sampler)
{
    auto var = GetRootVar()[name];
    if (!IsSamplerVarValid(var, sampler.get()))
        return false;
    return var.SetSampler(sampler);
}

bool ParameterBlock::SetParameterBlock(const ShaderVarLocation &location, const SPtr<ParameterBlock> &block)
{
    CT_CHECK(block);

    if (!CheckResourceLocation(location))
    {
        LOG_WRN_RESOURCE_LOCATION(set, parameter block);
        return false;
    }

    if (!CheckDescriptorType(location, DescriptorType::Cbv))
    {
        LOG_WRN_DESCRIPTOR_TYPE(set, parameter block);
        return false;
    }

    auto flatIndex = GetFlatIndex(location);
    if (parameterBlocks[flatIndex] == block)
        return true;
    parameterBlocks[flatIndex] = block;
    MarkDescriptorSetDirty(location);

    block->parent = this;

    return true;
}

bool ParameterBlock::SetParameterBlock(const String &name, const SPtr<ParameterBlock> &block)
{
    auto var = GetRootVar()[name];
    if (!IsParameterBlockVarValid(var, block.get()))
        return false;
    return var.SetParameterBlock(block);
}

bool ParameterBlock::SetBlob(const void *data, uint32 offset, uint32 size)
{
    CT_CHECK(offset + size <= constBufferData.Count());

    std::memcpy(constBufferData.GetData() + offset, data, size);
    MarkUniformDataDirty();
    return true;
}

bool ParameterBlock::SetBlob(const ShaderVarLocation &location, const void *data, uint32 size)
{
    return SetBlob(data, location.byteOffset, size);
}

void ParameterBlock::UpdateConstantBuffer()
{
    if (reflection->HasConstantBuffer())
    {
        if (constantBufferDirty)
        {
            constantBufferDirty = false;

            auto buffer = GetUnderlyingConstantBuffer().get();
            uint8 *data = (uint8 *)buffer->Map(BufferMapType::WriteDiscard);
            std::memcpy(data, constBufferData.GetData(), constBufferData.Count());
            buffer->Unmap();
        }
    }
}

static void PrepareResource(CopyContext *ctx, Resource *resource, bool isUav)
{
    if (!resource)
        return;

    auto buffer = resource->AsBuffer().get();
    if (buffer && isUav && buffer->GetUavCounter())
    {
        ctx->ResourceBarrier(buffer->GetUavCounter().get(), ResourceState::UnorderedAccess);
    }

    bool insertBarrier = ctx->ResourceBarrier(resource, isUav ? ResourceState::UnorderedAccess : ResourceState::ShaderResource);

    if (insertBarrier && isUav)
    {
        ctx->UavBarrier(resource);
    }
}

bool ParameterBlock::PrepareResources(CopyContext *ctx)
{
    for (auto &srv : srvs)
    {
        if (srv)
        {
            auto resource = srv->GetResource();
            PrepareResource(ctx, resource, false);
        }
    }

    for (auto &uav : uavs)
    {
        if (uav)
        {
            auto resource = uav->GetResource();
            PrepareResource(ctx, resource, true);
        }
    }

    for (auto &block : parameterBlocks)
    {
        if (block)
        {
            block->PrepareResourcesAndConstantBuffer(ctx);
        }
    }

    return true;
}

bool ParameterBlock::PrepareResourcesAndConstantBuffer(CopyContext *ctx)
{
    UpdateConstantBuffer();
    return PrepareResources(ctx);
}

bool ParameterBlock::BindIntoDescriptorSet(int32 setIndex, const SPtr<DescriptorSet> &set)
{
    if (reflection->HasConstantBuffer())
    {
        auto bindingInfo = reflection->GetConstantBufferBindingInfo();
        if (setIndex == bindingInfo.set)
        {
            set->SetCbv(bindingInfo.binding, 0, GetUnderlyingConstantBuffer()->GetCbv().get());
        }
    }

    if (reflection->GetSetInfoCount() <= setIndex) // sub block does not contain bindings with this setIndex
    {
        return true;
    }

    auto setInfo = reflection->GetSetInfo(setIndex);
    for (auto index : setInfo.bindingIndices)
    {
        auto &bindingInfo = reflection->GetBindingInfo(index);
        auto &range = GetElementType()->GetBindingRange(index);
        for (int32 i = 0; i < range.count; ++i)
        {
            int32 flatIndex = range.baseIndex + i;
            switch (range.descriptorType)
            {
            case DescriptorType::Cbv:
            {
                CT_CHECK(range.count == 1);
                CT_CHECK(parameterBlocks[flatIndex]);
                if (!parameterBlocks[flatIndex]->BindIntoDescriptorSet(setIndex, set))
                    return false;
            }
            break;
            case DescriptorType::Sampler:
            {
                auto sampler = samplers[flatIndex] ? samplers[flatIndex].get() : Sampler::GetDefault().get();
                set->SetSampler(bindingInfo.binding, i, sampler);
            }
            break;
            case DescriptorType::TextureSrv:
            case DescriptorType::RawBufferSrv:
            case DescriptorType::TypedBufferSrv:
            case DescriptorType::StructuredBufferSrv:
            {
                if (!srvs[flatIndex])
                {
                    CT_LOG(Error, CT_TEXT("ParameterBlock try to bind null srv into descriptor set, name: {0}."), bindingInfo.name);
                    return false;
                }
                set->SetSrv(bindingInfo.binding, i, srvs[flatIndex].get());
            }
            break;
            case DescriptorType::TextureUav:
            case DescriptorType::RawBufferUav:
            case DescriptorType::TypedBufferUav:
            case DescriptorType::StructuredBufferUav:
            {
                if (!uavs[flatIndex])
                {
                    CT_LOG(Error, CT_TEXT("ParameterBlock try to bind null uav into descriptor set, name: {0}."), bindingInfo.name);
                    return false;
                }
                set->SetUav(bindingInfo.binding, i, uavs[flatIndex].get());
            }
            break;
            default:
                break;
            }
        }
    }

    return true;
}

bool ParameterBlock::PrepareDescriptorSets(CopyContext *ctx)
{
    if (!PrepareResourcesAndConstantBuffer(ctx))
        return false;

    //Only main block contains descriptor set.
    for (int32 i = 0; i < reflection->GetSetInfoCount(); ++i)
    {
        if (reflection->GetSetInfo(i).bindingIndices.Count() == 0)
            continue;

        if (!sets[i])
        {
            auto set = DescriptorSet::Create(RenderAPI::GetDevice()->GetGpuDescriptorPool(), reflection->GetDescriptorSetLayout(i));
            if (!BindIntoDescriptorSet(i, set))
            {
                return false;
            }
            sets[i] = set;
        }
    }
    return true;
}

//===========================================================================

ShaderVar ShaderVar::FindMember(const String &name) const
{
    if (!IsValid())
        return ShaderVar();

    auto varType = GetVarType();

    if (auto resourceType = varType->AsResource())
    {
        if (resourceType->GetShaderResourceType() == ShaderResourceType::ConstantBuffer)
        {
            return GetParameterBlock()->GetRootVar().FindMember(name);
        }
    }
    if (auto structType = varType->AsStruct())
    {
        auto member = structType->GetMember(name);
        if (member)
        {
            auto memberLoc = ShaderVarLocation(member->GetReflectionType(), location + member->GetLocation());
            return ShaderVar(block, memberLoc);
        }
    }
    return ShaderVar();
}

ShaderVar ShaderVar::FindMember(int32 index) const
{
    if (!IsValid())
        return ShaderVar();

    auto varType = GetVarType();

    if (auto resourceType = varType->AsResource())
    {
        if (resourceType->GetShaderResourceType() == ShaderResourceType::ConstantBuffer)
        {
            return GetParameterBlock()->GetRootVar().FindMember(index);
        }
    }
    else if (auto structType = varType->AsStruct())
    {
        auto member = structType->GetMember(index);
        if (member)
        {
            auto memberLoc = ShaderVarLocation(member->GetReflectionType(), location + member->GetLocation());
            return ShaderVar(block, memberLoc);
        }
    }
    else if (auto arrayType = varType->AsArray())
    {
        if (index >= 0 && index < arrayType->GetElementCount())
        {
            VarLocation newLoc;
            newLoc.rangeIndex = location.rangeIndex;
            newLoc.arrayIndex = index;
            newLoc.byteOffset = location.byteOffset + arrayType->GetStride() * index;
            return ShaderVar(block, ShaderVarLocation(arrayType->GetElementType(), newLoc));
        }
    }
    return ShaderVar();
}

SPtr<Buffer> ShaderVar::GetBuffer() const
{
    return block->GetBuffer(location);
}

SPtr<Texture> ShaderVar::GetTexture() const
{
    return block->GetTexture(location);
}

SPtr<ResourceView> ShaderVar::GetSrv() const
{
    return block->GetSrv(location);
}

SPtr<ResourceView> ShaderVar::GetUav() const
{
    return block->GetUav(location);
}

SPtr<Sampler> ShaderVar::GetSampler() const
{
    return block->GetSampler(location);
}

SPtr<ParameterBlock> ShaderVar::GetParameterBlock() const
{
    return block->GetParameterBlock(location);
}

bool ShaderVar::SetBuffer(const SPtr<Buffer> &buffer) const
{
    return block->SetBuffer(location, buffer);
}

bool ShaderVar::SetTexture(const SPtr<Texture> &texture) const
{
    return block->SetTexture(location, texture);
}

bool ShaderVar::SetSrv(const SPtr<ResourceView> &srv) const
{
    return block->SetSrv(location, srv);
}

bool ShaderVar::SetUav(const SPtr<ResourceView> &uav) const
{
    return block->SetUav(location, uav);
}

bool ShaderVar::SetSampler(const SPtr<Sampler> &sampler) const
{
    return block->SetSampler(location, sampler);
}

bool ShaderVar::SetParameterBlock(const SPtr<ParameterBlock> &newBlock) const
{
    return block->SetParameterBlock(location, newBlock);
}

bool ShaderVar::SetBlob(const void *data, uint32 size) const
{
    auto varType = GetVarType();
    if (auto resourceType = varType->AsResource())
    {
        switch (resourceType->GetShaderResourceType())
        {
        case ShaderResourceType::ConstantBuffer:
            return GetParameterBlock()->GetRootVar().SetBlob(data, size);
        default:
            break;
        }
    }

    return block->SetBlob(location, data, size);
}

ShaderVar::operator SPtr<Buffer>() const
{
    return block->GetBuffer(location);
}

ShaderVar::operator SPtr<Texture>() const
{
    return block->GetTexture(location);
}

ShaderVar::operator SPtr<Sampler>() const
{
    return block->GetSampler(location);
}

ShaderVar ShaderVar::operator[](const String &name) const
{
    auto ret = FindMember(name);
    if (!ret.IsValid() && IsValid())
    {
        CT_LOG(Error, CT_TEXT("ShaderVar[] attempt to find invalid member, name is {0}."), name);
    }
    return ret;
}

ShaderVar ShaderVar::operator[](int32 index) const
{
    auto ret = FindMember(index);
    if (!ret.IsValid() && IsValid())
    {
        CT_LOG(Error, CT_TEXT("ShaderVar[] attempt to find invalid member, index is {0}."), index);
    }
    return ret;
}
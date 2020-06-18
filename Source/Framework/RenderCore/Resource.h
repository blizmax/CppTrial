#pragma once

#include "RenderCore/ResourceView.h"

class Resource
{
public:
    Resource(ResourceType type, ResourceBindFlags bindFlags, uint32 size)
        : resourceType(type), bindFlags(bindFlags), size(size)
    {
    }

    virtual ~Resource() = default;

    uint32 GetSize() const
    {
        return size;
    }

    ResourceType GetResourceType() const
    {
        return resourceType;
    }

    ResourceBindFlags GetBindFlags() const
    {
        return bindFlags;
    }

    bool IsStateGlobal() const
    {
        return stateData.global;
    }

    ResourceState GetGlobalState() const
    {
        CT_CHECK(stateData.global);
        return stateData.state;
    }

    void SetGlobalState(ResourceState newState) const
    {
        stateData.global = true;
        stateData.state = newState;
    }

    virtual ResourceState GetSubresourceState(int32 arraySlice, int32 mipLevel) const = 0;
    virtual void SetSubresourceState(int32 arraySlice, int32 mipLevel, ResourceState newState) const = 0;

    virtual SPtr<ResourceView> GetSrv() = 0;
    virtual SPtr<ResourceView> GetUav() = 0;

    virtual SPtr<Buffer> AsBuffer()
    {
        return nullptr;
    }

    virtual SPtr<Texture> AsTexture()
    {
        return nullptr;
    }

protected:
    struct StateData
    {
        ResourceState state = ResourceState::Undefined;
        Array<ResourceState> subStates;
        bool global = true;
    };

    ResourceType resourceType;
    ResourceBindFlags bindFlags = 0;
    mutable StateData stateData;
    uint32 size = 0;

};
#pragma once

#include "Assets/.Package.h"
#include "Assets/AssetObject.h"
#include "Core/Thread.h"

class AssetHandleBase
{
public:
    struct InnerData
    {
        SPtr<AssetObject> ptr;
        std::atomic<uint32> refCount{0};
        bool done = false;
    };

    bool IsLoaded() const
    {
        if (data && data->ptr && data->done)
        {
            return true;
        }
        return false;
    }

    bool IsValid() const
    {
        if (data && data->ptr)
        {
            return true;
        }
        return false;
    }

    void Release();
    void BlockUntilLoaded();

protected:
    void Destroy();

    void IncRef()
    {
        if (data)
        {
            data->refCount.fetch_add(1);
        }
    }

    void DecRef()
    {
        if (data)
        {
            uint32 c = data->refCount.fetch_sub(1);
            if (c == 1)
            {
                Destroy();
            }
        }
    }

protected:
    SPtr<InnerData> data;
};

template <typename T>
class AssetHandle : public AssetHandleBase
{
public:
    friend class AssetHandle<AssetObject>;

    AssetHandle() = default;
    AssetHandle(AssetHandle &&) noexcept = default;
    AssetHandle &operator=(AssetHandle &&) noexcept = default;

    AssetHandle(const AssetHandle &other)
    {
        SetData(other.data);
    }

    AssetHandle &operator=(const AssetHandle &other)
    {
        if (this != &other)
        {
            AssetHandle temp(other);
            Swap(temp);
        }
        return *this;
    }

    ~AssetHandle()
    {
        DecRef();
    }

    void Swap(AssetHandle &other) noexcept
    {
        std::swap(data, other.data);
    }

    void SetData(const SPtr<InnerData> &newData)
    {
        DecRef();
        data = newData;
        IncRef();
    }

    T *Get() const
    {
        if (!IsValid())
        {
            CT_EXCEPTION(Assets, "Trying to access an invalid asset.");
            return nullptr;
        }

        return reinterpret_cast<T *>(data->ptr.get());
    }

    template <typename U>
    AssetHandle<U> Cast() const
    {
        AssetHandle<AssetObject> result;
        result.SetData(data);

        return result;
    }

    T *operator->() const
    {
        return Get();
    }

    T &operator*() const
    {
        return *Get();
    }

    explicit operator bool() const
    {
        return IsValid();
    }
};
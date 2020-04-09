#pragma once

#include "Assets/.Package.h"
#include "Assets/AssetObject.h"
#include "Core/Thread.h"
#include "Utils/UUID.h"

class AssetPtrBase
{
public:
    struct InnerData
    {
        SPtr<AssetObject> ptr;
        std::atomic<uint32> refCount{0};
        UUID uuid;
        bool done = false;
    };

    AssetObject *Get() const
    {
        if (!data)
            return nullptr;
        return data->ptr.get();
    }

    bool IsPending() const
    {
        if (data && data->done)
            return true;
        return false;
    }

    bool IsValid() const
    {
        return Get() != nullptr;
    }

    void LoadSync();

    void Release();

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
class AssetPtr : public AssetPtrBase
{
public:
    friend class AssetPtr<AssetObject>;

    AssetPtr() = default;
    AssetPtr(AssetPtr &&) noexcept = default;
    AssetPtr &operator=(AssetPtr &&) noexcept = default;

    AssetPtr(std::nullptr_t) {}

    AssetPtr(const AssetPtr &other)
    {
        SetData(other.data);
    }

    AssetPtr &operator=(const AssetPtr &other)
    {
        if (this != &other)
        {
            AssetPtr temp(other);
            Swap(temp);
        }
        return *this;
    }

    ~AssetPtr()
    {
        DecRef();
    }

    void Swap(AssetPtr &other) noexcept
    {
        std::swap(data, other.data);
    }

    void SetData(const SPtr<InnerData> &newData)
    {
        if (newData != data)
        {
            DecRef();
            data = newData;
            IncRef();
        }
    }

    T *Get() const
    {
        return reinterpret_cast<T *>(AssetPtr::Get());
    }

    template <typename U>
    AssetPtr<U> Cast() const
    {
        AssetPtr<AssetObject> result;
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

    template <typename T1, typename T2>
    friend bool operator==(const AssetPtr<T1> &lhs, const AssetPtr<T2> &rhs)
    {
        return lhs.data == rhs.data;
    }

    template <typename T1, typename T2>
    friend bool operator!=(const AssetPtr<T1> &lhs, const AssetPtr<T2> &rhs)
    {
        return lhs.data != rhs.data;
    }

    friend bool operator!=(const AssetPtr<T> &lhs, std::nullptr_t)
    {
        return lhs.IsValid();
    }

    friend bool operator!=(std::nullptr_t, const AssetPtr<T> &rhs)
    {
        return rhs.IsValid();
    }
};

template <typename T>
using APtr = AssetPtr<T>;
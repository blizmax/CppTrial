#pragma once

#include "Assets/.Package.h"
#include "Core/Thread.h"

template <typename T>
class AssetPtr
{
public:
    struct InnerData
    {
        SPtr<T> ptr;

        InnerData() = default;
        InnerData(const SPtr<T> &ptr)
            : ptr(ptr)
        {
        }

        static SPtr<InnerData> Create()
        {
            return Memory::MakeShared<InnerData>();
        }

        static SPtr<InnerData> Create(const SPtr<T> &ptr)
        {
            return Memory::MakeShared<InnerData>(ptr);
        }
    };

    AssetPtr() = default;
    ~AssetPtr() = default;

    AssetPtr(AssetPtr &&) noexcept = default;
    AssetPtr &operator=(AssetPtr &&) noexcept = default;

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

    AssetPtr(const SPtr<T> &ptr)
    {
        SetData(InnerData::Create(ptr));
    }

    AssetPtr(std::nullptr_t)
    {
    }

    T *Get() const
    {
        if (!data)
            return nullptr;
        return data->ptr.get();
    }

    bool IsValid() const
    {
        return Get() != nullptr;
    }

    void Swap(AssetPtr &other) noexcept
    {
        std::swap(data, other.data);
    }

    void NewData()
    {
        data = InnerData::Create();
    }

    void SetData(const SPtr<InnerData> &newData)
    {
        if (newData != data)
        {
            data = newData;
        }
    }

    const SPtr<InnerData> &GetData() const
    {
        return data;
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

    operator SPtr<T>() const
    {
        if (!data)
            return nullptr;
        return data->ptr;
    }

private:
    SPtr<InnerData> data;
};

template <typename T1, typename T2>
CT_INLINE bool operator==(const AssetPtr<T1> &lhs, const AssetPtr<T2> &rhs)
{
    return lhs.GetData() == rhs.GetData();
}

template <typename T1, typename T2>
CT_INLINE bool operator!=(const AssetPtr<T1> &lhs, const AssetPtr<T2> &rhs)
{
    return lhs.GetData() != rhs.GetData();
}

template <typename T>
CT_INLINE bool operator!=(const AssetPtr<T> &lhs, std::nullptr_t)
{
    return lhs.IsValid();
}

template <typename T>
CT_INLINE bool operator!=(std::nullptr_t, const AssetPtr<T> &rhs)
{
    return rhs.IsValid();
}

template <typename T>
using APtr = AssetPtr<T>;

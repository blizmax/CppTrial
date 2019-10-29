#pragma once

#include "Core/General.h"
#include "Core/Memory.h"

CT_SCOPE_BEGIN

namespace AnyInternal
{
class IDynamicData
{
public:
    virtual ~IDynamicData() = default;
    virtual IDynamicData *Clone() const = 0;
};

template <typename T>
class DynamicData : public IDynamicData
{
public:
    T value;

    DynamicData(const T &val) : value(val)
    {
    }

    DynamicData(T &&val) : value(std::move(val))
    {
    }

    IDynamicData *Clone() const override
    {
        return Memory::New<DynamicData>(value);
    }
};
} // namespace AnyInternal

class Any
{
public:
    Any() = default;

    Any(const Any &other)
    {
        if (other.data)
            data = other.data->Clone();
    }

    Any(Any &&other) : data(other.data)
    {
        other.data = nullptr;
    }

    Any &operator=(const Any &other)
    {
        if (this != &other)
        {
            Any temp(other);
            Swap(temp);
        }
        return *this;
    }

    Any &operator=(Any &&other)
    {
        if (this != &other)
        {
            Any temp(std::move(other));
            Swap(temp);
        }
        return *this;
    }

    template <typename T>
    Any(T *value) : data(Memory::New<AnyInternal::DynamicData<typename TDecay<T *>::type>>(value))
    {
    }

    template <typename T, typename = typename TEnableIf<!TIsPointer<T>::value, T>::type>
    Any(const T &value) : data(Memory::New<AnyInternal::DynamicData<typename TDecay<T>::type>>(value))
    {
    }

    template <typename T>
    Any(T &value) : data(Memory::New<AnyInternal::DynamicData<typename TDecay<T>::type>>(value))
    {
    }

    template <typename T>
    Any &operator=(T &&value)
    {
        Any temp(std::forward<T>(value));
        Swap(temp);
        return *this;
    }

    ~Any()
    {
        Clear();
    }

    bool IsEmpty() const
    {
        return data == nullptr;
    }

    void Swap(Any &other)
    {
        std::swap(data, other.data);
    }

    void Clear()
    {
        if (data)
            Memory::Delete(data);
        data = nullptr;
    }

    template <typename T>
    T Cast() const
    {
        if (!data)
            CT_THROW("Invalid to cast an empty any.");
        return static_cast<AnyInternal::DynamicData<T> *>(data)->value;
    }

    template <typename T>
    T &RefCast()
    {
        if (!data)
            CT_THROW("Invalid to cast an empty any.");
        return static_cast<AnyInternal::DynamicData<T> *>(data)->value;
    }

    template <typename T>
    const T &RefCast() const
    {
        if (!data)
            CT_THROW("Invalid to cast an empty any.");
        return static_cast<AnyInternal::DynamicData<T> *>(data)->value;
    }

    template <typename T>
    operator T() const
    {
        return Cast<T>();
    }

private:
    AnyInternal::IDynamicData *data = nullptr;
};

CT_SCOPE_END
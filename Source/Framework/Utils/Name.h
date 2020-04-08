#pragma once

#include "Core/String.h"

namespace NameInternal
{
struct Data
{
    int32 hash;
    String string;
};
} // namespace NameInternal

class Name
{
public:
    using Data = NameInternal::Data;

    Name() = default;
    ~Name() = default; // So no need to remove existing name.

    Name(const Name &other);
    Name(Name &&other);
    Name &operator=(const Name &other);
    Name &operator=(Name &&other);

    Name(const String &value);
    Name(const CharType *value);

    bool IsEmpty() const
    {
        return data == nullptr;
    }

    void Swap(Name &other)
    {
        std::swap(data, other.data);
    }

    String ToString() const
    {
        return data ? data->string : String();
    }

    uint32 HashCode() const
    {
        return data ? data->hash : 0;
    }

    bool operator==(const Name &other) const
    {
        if (data)
            return data == other.data;
        return other.data == nullptr;
    }

    bool operator!=(const Name &other) const
    {
        return *this == other;
    }

    static const Data *Find(const String &value);

#ifdef CT_DEBUG
    static Array<Data *> DebugDumpNameMap();
#endif

private:
    void Construct(const String &value);

private:
    Data *data = nullptr;
};

namespace std
{
inline void swap(Name &lhs, Name &rhs)
{
    lhs.Swap(rhs);
}
}
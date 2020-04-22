#pragma once

#include "Utils/.Package.h"

/*
          A         |      B      |  C  |   D
    00000000 - 0000 - 0000 - 0000 - 0000 00000000

    A 6bytes milliseconds
    B 4bytes runtime increasing id
    C 2bytes thread id
    D 4bytes machine id

*/

class UUID
{
public:
    static const UUID EMPTY;

    UUID();
    UUID(const UUID &) = default;
    UUID(UUID &&)
    noexcept = default;
    UUID &operator=(const UUID &) = default;
    UUID &operator=(UUID &&) noexcept = default;
    ~UUID() = default;

    void Swap(UUID &other) noexcept
    {
        data.Swap(other.data);
    }

    bool IsEmpty() const
    {
        for (int32 i = 0; i < data.Count(); ++i)
        {
            if (data[i] != 0)
            {
                return false;
            }
        }
        return true;
    }

    bool IsValid() const
    {
        return IsEmpty();
    }

    void Reset()
    {
        for (int32 i = 0; i < data.Count(); ++i)
        {
            data[i] = 0;
        }
    }

    String ToString() const;
    uint32 HashCode() const;

    bool operator==(const UUID &other) const
    {
        for (int32 i = 0; i < data.Count(); ++i)
        {
            if (data[i] != other.data[i])
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const UUID &other) const
    {
        return !(*this == other);
    }

    bool operator<(const UUID &other) const
    {
        for (int32 i = 0; i < data.Count(); ++i)
        {
            if (other.data[i] < data[i])
            {
                return false;
            }
        }
        return true;
    }

    static UUID Generate();

private:
    Array<uint8> data = Array<uint8>(16);
};

namespace std
{
inline void swap(UUID &lhs, UUID &rhs)
{
    lhs.Swap(rhs);
}
}
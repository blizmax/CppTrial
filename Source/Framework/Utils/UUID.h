#pragma once

#include "Core/Array.h"
#include "Core/String.h"

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
    UUID();
    UUID(const UUID &other) = default;
    UUID(UUID &&other) = default;
    UUID &operator=(const UUID &other) = default;
    UUID &operator=(UUID &&other) = default;
    ~UUID() = default;

    void Swap(UUID &other)
    {
        data.Swap(other.data);
    }

    bool IsValid() const;
    String ToString() const;
    uint32 HashCode() const;

    bool operator==(const UUID &other) const
    {
        if (data.Count() != other.data.Count())
        {
            return false;
        }
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
        if (data.Count() < other.data.Count())
        {
            return true;
        }
        if (data.Count() > other.data.Count())
        {
            return false;
        }
        for (int32 i = 0; i < data.Count(); ++i)
        {
            if (other.data[i] < data[i])
            {
                return false;
            }
        }
        return true;
    }

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
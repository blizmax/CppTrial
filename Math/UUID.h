#pragma once

#include "Core/General.h"
#include "Core/Array.h"
#include "Core/String.h"

CT_SCOPE_BEGIN

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

    void Swap(UUID& other);
    bool IsValid() const;
    String ToString() const;
    uint32 HashCode() const;

    bool operator==(const UUID &other) const;
    bool operator!=(const UUID &other) const;
    bool operator<(const UUID &other) const;

private:
    Array<uint8> data = Array<uint8>(16); 
 
};

CT_SCOPE_END
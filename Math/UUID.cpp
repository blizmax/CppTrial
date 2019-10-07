#include "Core/General.h"
#include "Core/Threading.h"
#include "Core/Hash.h"
#include "Core/Time.h"
#include "Math/UUID.h"
#include "Math/Random.h"

CT_SCOPE_BEGIN

constexpr const CharType HEX_TO_CHAR[16] =
    {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9',
     L'a', L'b', L'c', L'd', L'e', L'f'};

static uint32 NextRuntimeID()
{
    static uint32 runTimeID = Math::Rand();
    return runTimeID++;
}

static uint32 GetMachineID()
{
    static uint32 fakeID = Math::Rand();
    return fakeID;
}

UUID::UUID()
{
    uint64 time = Time::MilliTime();
    for (int32 i = 5; i >= 0; --i)
    {
        uint8 v = (uint8)((time >> (i * 8)) & 0xFF);
        data.Add(v);
    }

    uint32 runtimeId = NextRuntimeID();
    for (int32 i = 3; i >= 0; --i)
    {
        uint8 v = (uint8)((runtimeId >> (i * 8)) & 0xFF);
        data.Add(v);
    }

    uint32 threadId = GetCurrentThreadID();
    for (int32 i = 1; i >= 0; --i)
    {
        uint8 v = (uint8)((threadId >> (i * 8)) & 0xFF);
        data.Add(v);
    }

    uint32 machineId = GetMachineID();
    for (int32 i = 3; i >= 0; --i)
    {
        uint8 v = (uint8)((machineId >> (i * 8)) & 0xFF);
        data.Add(v);
    }
}

bool UUID::IsValid() const
{
    return data.Size() == 16;
}

String UUID::ToString() const
{
    if (!IsValid())
    {
        return String();
    }

    CharType output[37];
    output[8] = output[13] = output[18] = output[23] = L'-';
    output[36] = 0;

    for (int32 i = 0; i < 4; ++i)
    {
        output[i * 2] = HEX_TO_CHAR[(data[i] >> 4) & 0xF];
        output[i * 2 + 1] = HEX_TO_CHAR[data[i] & 0xF];
    }
    for (int32 i = 4; i < 6; ++i)
    {
        output[i * 2 + 1] = HEX_TO_CHAR[(data[i] >> 4) & 0xF];
        output[i * 2 + 2] = HEX_TO_CHAR[data[i] & 0xF];
    }
    for (int32 i = 6; i < 8; ++i)
    {
        output[i * 2 + 2] = HEX_TO_CHAR[(data[i] >> 4) & 0xF];
        output[i * 2 + 3] = HEX_TO_CHAR[data[i] & 0xF];
    }
    for (int32 i = 8; i < 10; ++i)
    {
        output[i * 2 + 3] = HEX_TO_CHAR[(data[i] >> 4) & 0xF];
        output[i * 2 + 4] = HEX_TO_CHAR[data[i] & 0xF];
    }
    for (int32 i = 10; i < 16; ++i)
    {
        output[i * 2 + 4] = HEX_TO_CHAR[(data[i] >> 4) & 0xF];
        output[i * 2 + 5] = HEX_TO_CHAR[data[i] & 0xF];
    }

    return String(output);
}

uint32 UUID::HashCode() const
{
    uint32 hash = 0;
    for (auto &v : data)
    {
        Hash::HashCombine(hash, v);
    }
    return hash;
}

CT_SCOPE_END
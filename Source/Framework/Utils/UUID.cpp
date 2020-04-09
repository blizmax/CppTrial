#include "Utils/UUID.h"
#include "Core/Thread.h"
#include "Core/Hash.h"
#include "Core/Time.h"
#include "Core/Math.h"

constexpr const CharType HEX_TO_CHAR[16] =
    {CT_TEXT('0'), CT_TEXT('1'), CT_TEXT('2'), CT_TEXT('3'), CT_TEXT('4'),
     CT_TEXT('5'), CT_TEXT('6'), CT_TEXT('7'), CT_TEXT('8'), CT_TEXT('9'),
     CT_TEXT('a'), CT_TEXT('b'), CT_TEXT('c'), CT_TEXT('d'), CT_TEXT('e'), CT_TEXT('f')};

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

const UUID UUID::EMPTY;

UUID UUID::Generate()
{
    UUID result;

    uint64 time = Time::MilliTime();
    for (int32 i = 5; i >= 0; --i)
    {
        uint8 v = (uint8)((time >> (i * 8)) & 0xFF);
        result.data.Add(v);
    }

    uint32 runtimeId = NextRuntimeID();
    for (int32 i = 3; i >= 0; --i)
    {
        uint8 v = (uint8)((runtimeId >> (i * 8)) & 0xFF);
        result.data.Add(v);
    }

    uint32 threadId = GetCurrentThreadID();
    for (int32 i = 1; i >= 0; --i)
    {
        uint8 v = (uint8)((threadId >> (i * 8)) & 0xFF);
        result.data.Add(v);
    }

    uint32 machineId = GetMachineID();
    for (int32 i = 3; i >= 0; --i)
    {
        uint8 v = (uint8)((machineId >> (i * 8)) & 0xFF);
        result.data.Add(v);
    }

    return result;
}

UUID::UUID()
{
    Reset();
}

String UUID::ToString() const
{
    CharType output[37];
    output[8] = output[13] = output[18] = output[23] = CT_TEXT('-');
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
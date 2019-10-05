#include "Core/General.h"
#include "Core/Threading.h"
#include "Core/Hash.h"
#include "Core/Time.h"
#include "Math/UUID.h"
#include "Math/Random.h"

CT_SCOPE_BEGIN

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
    for(uint32 i = 0; i < 6; ++i)
    {
        uint8 v = (uint8)((time >> i) & 0xFF);
        data.Add(v);
    }

    uint32 runtimeId = NextRuntimeID();
    for(uint32 i = 0; i < 4; ++i)
    {
        uint8 v = (uint8)((runtimeId >> i) & 0xFF);
        data.Add(v);
    }

    uint32 threadId = GetCurrentThreadID();
    for(uint32 i = 0; i < 2; ++i)
    {
        uint8 v = (uint8)((threadId >> i) & 0xFF);
        data.Add(v);
    }

    uint32 machineId = GetMachineID(); 
    for (uint32 i = 0; i < 4; ++i)
    {
        uint8 v = (uint8)((machineId >> i) & 0xFF);
        data.Add(v);
    }
}


CT_SCOPE_END
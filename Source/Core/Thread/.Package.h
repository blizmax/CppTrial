#pragma once

#include "Core/.Package.h"
#include "Core/Time.h"
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <thread>

namespace Thread
{
CT_INLINE uint32 GetCurrentThreadID()
{
    std::thread::id threadId = std::this_thread::get_id();
    std::hash<std::thread::id> hash;
    return static_cast<uint32>(hash(threadId));
}

CT_INLINE uint32 HardwareConcurrency()
{
    return std::thread::hardware_concurrency();
}

CT_INLINE void SleepFor(int32 ms)
{
    std::this_thread::sleep_for(Time::Milliseconds(ms));
}

CT_INLINE void YieldThis()
{
    std::this_thread::yield();
}
}
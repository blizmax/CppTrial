#pragma once

#include "Core/.Package.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>

CT_INLINE uint32 GetCurrentThreadID()
{
    std::thread::id threadId = std::this_thread::get_id();
    std::hash<std::thread::id> hash;
    return static_cast<uint32>(hash(threadId));
}
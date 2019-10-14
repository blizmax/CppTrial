#pragma once

#include "Core/General.h"
#include <thread>
#include <atomic>
#include <mutex>

CT_SCOPE_BEGIN

CT_INLINE uint32 GetCurrentThreadID()
{
    std::thread::id threadId = std::this_thread::get_id();
    std::hash<std::thread::id> hash;
    return static_cast<uint32>(hash(threadId));
}

CT_SCOPE_END
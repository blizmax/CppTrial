#pragma once

#include "Core/General.h"
#include <chrono>

CT_SCOPE_BEGIN

namespace Time
{
using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Nanoseconds = std::chrono::nanoseconds;
using Microseconds = std::chrono::microseconds;
using Milliseconds = std::chrono::milliseconds;
using Seconds = std::chrono::seconds;

CT_INLINE TimePoint Now()
{
    return Clock::now();
}

CT_INLINE int64 NanoTime(const TimePoint &point)
{
    return point.time_since_epoch().count();
}

CT_INLINE int64 NanoTime()
{
    return NanoTime(Now());
}

CT_INLINE int64 MicroTime(const TimePoint &point)
{
    return std::chrono::duration_cast<Microseconds>(point.time_since_epoch()).count();
}

CT_INLINE int64 MicroTime()
{
    return MicroTime(Now());
}

CT_INLINE int64 MilliTime(const TimePoint &point)
{
    return std::chrono::duration_cast<Milliseconds>(point.time_since_epoch()).count();
}

CT_INLINE int64 MilliTime()
{
    return MilliTime(Now());
}
} // namespace TimeUtils

CT_SCOPE_END
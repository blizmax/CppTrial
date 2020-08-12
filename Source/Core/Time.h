#pragma once

#include "Core/String.h"
#include <chrono>
#include <ctime>

namespace Time
{

constexpr int32 MILLIS_PER_SECOND = 1000;
constexpr int32 MICROS_PER_SECOND = 1000000;
constexpr int32 NANOS_PER_SECOND = 1000000000;

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

CT_INLINE String ToString(const String &format, std::time_t value)
{
    wchar str[100];
    auto count = (int32)std::wcsftime(str, 100, format.CStr(), std::localtime(&value));
    return String(str, count);
}

CT_INLINE String ToString(const String &format)
{
    using SystemClock = std::chrono::system_clock;
    return ToString(format, SystemClock::to_time_t(SystemClock::now()));
}

}
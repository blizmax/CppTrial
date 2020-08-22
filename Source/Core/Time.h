#pragma once

#include "Core/String.h"
#include <chrono>
#include <ctime>

namespace Time
{

constexpr int32 SEC_TO_MILLI = 1000;
constexpr int32 SEC_TO_MICRO = 1000000;
constexpr int32 SEC_TO_NANO = 1000000000;
constexpr int32 MILLI_TO_MICRO = SEC_TO_MICRO / SEC_TO_MILLI;
constexpr int32 MILLI_TO_NANO = SEC_TO_NANO / SEC_TO_MILLI;
constexpr int32 MICRO_TO_NANO = SEC_TO_NANO / SEC_TO_MICRO;

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
    wchar buffer[128];
    auto count = (int32)std::wcsftime(buffer, 100, format.CStr(), std::localtime(&value));
    return String(buffer, count);
}

CT_INLINE String ToString(const String &format)
{
    using SystemClock = std::chrono::system_clock;
    return ToString(format, SystemClock::to_time_t(SystemClock::now()));
}

}